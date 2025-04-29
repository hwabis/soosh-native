#include "networking/server.h"
#include "handlers/game_message_handler.h"
#include "networking/server_session.h"
#include <boost/asio.hpp>
#include <iostream>

namespace soosh {

Server::Server(unsigned short port)
    : ioContext_(),
      acceptor_(ioContext_, ip::tcp::endpoint(ip::tcp::v4(), port)),
      messageHandler_(
          std::make_shared<GameMessageHandler>() // todo abstract this idk lol
      ) {}

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
          std::make_shared<ServerSession>(std::move(socket), messageHandler_)
              ->Start();
        } else {
          std::cerr << "[ERROR] Accept failed: " << ec.message() << '\n';
        }

        accept();
      });
}

} // namespace soosh
