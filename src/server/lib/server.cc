#include "server.h"
#include "session.h"
#include <boost/asio.hpp>
#include <iostream>

namespace soosh {

Server::Server(unsigned short port)
    : ioContext_(),
      acceptor_(ioContext_, ip::tcp::endpoint(ip::tcp::v4(), port)) {}

void Server::Start() {
  std::cout << "[INFO] Server started.\n";
  accept();
  ioContext_.run();
}

void Server::accept() {
  auto socket = std::make_shared<ip::tcp::socket>(ioContext_);
  acceptor_.async_accept(
      *socket, [this, socket](const boost::system::error_code &ec) {
        if (!ec) {
          std::make_shared<Session>(std::move(socket), GameMessageHandler())
              ->Start();
        } else {
          std::cerr << "[ERROR] Accept failed: " << ec.message() << '\n';
        }

        accept();
      });
}

} // namespace soosh
