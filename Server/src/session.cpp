#include <iostream>

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
        room_.deliver(read_msg_, socket_.remote_endpoint());
        read();
    }
    else
    {
        tcp::endpoint remote_dir = socket_.remote_endpoint();
        std::cout << remote_dir.address() << ":" << remote_dir.port() << " se ha desconectado del servidor" << std::endl;
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
