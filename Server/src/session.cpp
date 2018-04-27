#include <iostream>
#include <cstring>

#include <boost/bind.hpp>

#include "session.h"


Session::Session(tcp::socket socket, Room & room) :
    socket_(std::move(socket)),
    room_(room)
{
}


void Session::start()
{
    room_.join(shared_from_this());

    Message connection_message;
    connection_message.setMessage(std::string(getDirection().address().to_string() + ":" +
                                      std::to_string(getDirection().port()) + " se ha conectado al servidor.").c_str());

    std::cout << connection_message;
    room_.deliver(connection_message, getDirection());

    read();
}


void Session::deliver(const Message & msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_progress)
    {
        write();
    }
}


tcp::endpoint Session::getDirection() const
{
    return socket_.remote_endpoint();
}


void Session::read()
{
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.data(),
                                                read_msg_.max_length()),
                            boost::bind(&Session::handle_read, shared_from_this(),
                                        boost::asio::placeholders::error));
}


void Session::handle_read(const boost::system::error_code & error)
{
    if(!error)
    {
        if(read_msg_.compare("?:", 2))
            command();
        else
            room_.deliver(read_msg_, socket_.remote_endpoint());

        read();
    }
    else
    {
        Message disconnection_message;
        disconnection_message.setMessage(std::string(getDirection().address().to_string() + ":" +
                                                     std::to_string(getDirection().port()) +  " se ha desconectado del servidor.").c_str());

        std::cout << disconnection_message;
        room_.deliver(disconnection_message, getDirection());

        room_.leave(shared_from_this());
    }
}


void Session::write()
{
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().max_length()),
                             boost::bind(&Session::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error));
}


void Session::handle_write(const boost::system::error_code & error)
{
    if(!error)
    {
        write_msgs_.pop_front();
        if(!write_msgs_.empty())
        {
            write();
        }
    }
    else
    {
        room_.leave(shared_from_this());
    }
}


void Session::command()
{
    if(read_msg_.compare("?:lista", 7))
        std::cout << "Comando de lista" << std::endl;
}
