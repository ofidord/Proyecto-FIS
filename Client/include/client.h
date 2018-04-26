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

    boost::asio::io_service & io_service_;
    tcp::socket socket_;

    Message read_msg_;
    std::deque<Message> write_msgs_;

public:
    Client(boost::asio::io_service & io_service,
           const tcp::resolver::results_type & endpoints) :
        io_service_(io_service),
        socket_(io_service)
    {
        boost::asio::async_connect(socket_, endpoints, boost::bind(&Client::onConnect, this, _1));
    }

    void write(const Message & msg)
    {
        io_service_.post(boost::bind(&Client::writeImpl, this, msg));
    }

    void close()
    {
        io_service_.post(boost::bind(&Client::closeImpl, this));
    }

private:

    void onConnect(const boost::system::error_code & error)
    {
        if(!error)
        {
            Message msg;
            msg.setMessage("Connection");
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(msg.data(), msg.max_length()),
                                     boost::bind(&Client::readHandler, this, _1));
        }
    }

    void readHandler(const boost::system::error_code & error)
    {
        std::cout << read_msg_.data() << std::endl;

        if(!error)
        {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_.data(), read_msg_.max_length()),
                                    boost::bind(&Client::readHandler, this, _1));
        }
        else
        {
            closeImpl();
        }
    }

    void writeImpl(Message & msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);

        if(!write_in_progress)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().max_length()),
                                     boost::bind(&Client::writeHandler, this, _1));
        }
    }

    void writeHandler(const boost::system::error_code & error)
    {
        if(!error)
        {
            write_msgs_.pop_front();
            if(!write_msgs_.empty())
            {
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().max_length()),
                                         boost::bind(&Client::writeHandler, this, _1));
            }
        }
        else
        {
            closeImpl();
        }
    }

    void closeImpl()
    {
        socket_.close();
    }

};
