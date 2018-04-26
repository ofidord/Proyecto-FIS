#pragma once

#include <iostream>
#include <string>
#include <deque>

#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;

class Client
{
private:

    boost::asio::io_context & io_context_;
    tcp::socket socket_;

    Message read_msg_;
    std::deque<Message> write_msgs_;

public:
    Client(boost::asio::io_service & io_context,
           const tcp::resolver::results_type & endpoints) :
        io_context_(io_context),
        socket_(io_context)
    {
        connect(endpoints);
    }

    void write(const Message & msg)
    {
        boost::asio::post(io_context_,
            [this, msg]()
        {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if(!write_in_progress)
                write();
        });
    }

    void close()
    {
        boost::asio::post(io_context_, boost::bind(&Client::handle_close, this));
    }

private:

    void connect(const tcp::resolver::results_type & endpoints)
    {
        boost::asio::async_connect(socket_,endpoints,
                                   boost::bind(&Client::handle_connection, this,
                                               boost::asio::placeholders::error));
    }

    void handle_connection(const boost::system::error_code & error)
    {
        if(!error)
        {
            read();
        }
    }


    void read()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), read_msg_.max_length()),
                                boost::bind(&Client::handle_read, this, boost::asio::placeholders::error));
    }

    void handle_read(const boost::system::error_code & error)
    {
        if(!error)
        {
            std::cout << read_msg_;
            read();
        }
        else
        {
            socket_.close();
        }
    }


    void write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().max_length()),
                                 boost::bind(&Client::handle_write, this, boost::asio::placeholders::error));
    }

    void handle_write(const boost::system::error_code & error)
    {
        if(!error)
        {
            write_msgs_.pop_front();
            if(!write_msgs_.empty())
                write();
        }
        else
        {
            socket_.close();
        }
    }


    void handle_close()
    {
        socket_.close();
    }

};
