#include "server.h"

#include <iostream>
#include <memory>


Server::Server(boost::asio::io_context & io_context, const tcp::endpoint & endpoint) :
    io_context_(io_context),
    acceptor_(io_context, endpoint)
{
    accept();
}


void Server::accept()
{
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
            tcp::endpoint remote_dir = socket.remote_endpoint();
            std::cout << remote_dir.address() << ":" << remote_dir.port() << " se ha conectado al servidor." << std::endl;
            std::make_shared<Session>(std::move(socket), room_)->start();
        }

        accept();
      });
}
