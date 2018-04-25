#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    try
    {
        std::string ip;
        std::string port;

        if(argc != 3)
        {
            std::cout << "Ip: ";
            std::cin >> ip;
            std::cout << "Port: ";
            std::cin >> port;
        }
        else
        {
            ip = argv[1];
            port = argv[2];
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected!" << std::endl;

        boost::system::error_code ignored_error;

        Message msg("Hola");
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()), ignored_error);

    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
