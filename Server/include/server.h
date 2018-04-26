#pragma once

#include <algorithm>
#include <iostream>
#include <list>

#include <boost/asio.hpp>

#include <message.h>

using boost::asio::ip::tcp;

class chat_participant
{
public:
    virtual ~chat_participant() = default;
    virtual void deliver(const Message & msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room
{
private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    std::deque<Message> recent_msgs_;


public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        for(auto msg : recent_msgs_)
            participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const Message & msg)
    {
        recent_msgs_.push_back(msg);
        while(recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for(auto participant : participants_)
            participant->deliver(msg);
    }
};

class chat_session :
        public chat_participant,
        public std::enable_shared_from_this<chat_session>
{
private:
    tcp::socket socket_;
    chat_room & room_;
    Message read_msg_;
    std::deque<Message> write_msgs_;

public:
    chat_session(tcp::socket socket, chat_room & room) :
        socket_(std::move(socket)),
        room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        read();
    }

    void deliver(const Message & msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if(!write_in_progress)
        {
            write();
        }
    }

private:

    void read()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(),
                                                    read_msg_.max_length()),
                                boost::bind(&chat_session::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error));
    }


    void handle_read(const boost::system::error_code & error)
    {
        if(!error)
        {
            room_.deliver(read_msg_);
            read();
        }
        else
        {
            tcp::endpoint remote_dir = socket_.remote_endpoint();
            std::cout << remote_dir.address() << ":" << remote_dir.port() << " se ha desconectado del servidor" << std::endl;
            room_.leave(shared_from_this());
        }
    }

    void write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().max_length()),
                                 boost::bind(&chat_session::handle_write, shared_from_this(),
                                             boost::asio::placeholders::error));
    }

    void handle_write(const boost::system::error_code & error)
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
};


class Server
{
private:
    boost::asio::io_context & io_context_;
    tcp::acceptor acceptor_;
    chat_room room_;
public:
    Server(boost::asio::io_context & io_context, const tcp::endpoint & endpoint) :
        io_context_(io_context),
        acceptor_(io_context, endpoint)
    {
        accept();
    }

private:

    void accept()
    {
      acceptor_.async_accept(
          [this](boost::system::error_code ec, tcp::socket socket)
          {
            if (!ec)
            {
                tcp::endpoint remote_dir = socket.remote_endpoint();
                std::cout << remote_dir.address() << ":" << remote_dir.port() << " se ha conectado al servidor." << std::endl;
                std::make_shared<chat_session>(std::move(socket), room_)->start();
            }

            accept();
          });
    }
};
