#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "message.h"
#include "client.h"

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

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

        Client c(io_service, endpoints);

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        std::cin.clear();

        std::string line;
        while(std::getline(std::cin, line))
        {
            std::cout << "A" << std::endl;
            Message msg;
            msg.setMessage(line.c_str());

            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
