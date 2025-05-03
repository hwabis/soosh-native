#include "networking/server.h"
#include "handlers/game_message_handler.h"
#include "networking/client_session.h"
#include "utils/logger.h"
#include <boost/asio.hpp>

namespace soosh {

Server::Server(unsigned short port)
    : ioContext_(),
      acceptor_(ioContext_, ip::tcp::endpoint(ip::tcp::v4(), port)),
      messageHandler_(
          std::make_shared<GameMessageHandler>() // todo abstract this idk lol
      ) {}

void Server::Start() {
  Logger::Log("Server started.");
  accept();
  ioContext_.run();
}

void Server::accept() {
  auto socket = std::make_shared<ip::tcp::socket>(ioContext_);
  acceptor_.async_accept(
      *socket, [this, socket](const boost::system::error_code &ec) {
        if (!ec) {
          std::make_shared<ClientSession>(std::move(socket), messageHandler_)
              ->Start();
        } else {
          Logger::Log("Accept failed: " + ec.message(), Logger::Level::Error);
        }

        accept();
      });
}

} // namespace soosh
