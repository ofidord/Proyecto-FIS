#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;


int main(int argc, char *argv[])
{
    try
    {
        std::string port;

        if(argc != 2)
        {
            std::cout << "Port: ";
            std::cin >> port;
        }
        else
        {
            port = argv[1];
        }

        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)));

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Client connected!" << std::endl;

        while(true)
        {
            Message msg;
            boost::system::error_code ignored_error;
            boost::asio::read(socket, boost::asio::buffer(msg.data(), msg.max_length()), ignored_error);

            std::cout << msg;
        }
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
