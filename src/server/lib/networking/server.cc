#include "networking/server.h"
#include "handlers/game_message_handler.h"
#include "utils/logger.h"
#include <boost/asio.hpp>

namespace soosh {

Server::Server(unsigned short port)
    : ioContext_(),
      acceptor_(ioContext_, ip::tcp::endpoint(ip::tcp::v4(), port)),
      gameSession_(std::make_shared<GameSession>()) {}

void Server::Start() {
  Logger::Log("Server started.");
  messageHandler_ =
      std::make_shared<GameMessageHandler>(shared_from_this(), gameSession_);
  accept();
  ioContext_.run();
}

const std::vector<std::shared_ptr<ClientSession>> &Server::getSessions() const {
  return sessions_;
}

void Server::accept() {
  auto socket = std::make_shared<ip::tcp::socket>(ioContext_);
  acceptor_.async_accept(
      *socket, [this, socket](const boost::system::error_code &ec) {
        if (!ec) {
          auto session = std::make_shared<ClientSession>(std::move(socket),
                                                         messageHandler_);
          session->Start();
          sessions_.push_back(session);
        } else {
          Logger::Log("Accept failed: " + ec.message(), Logger::Level::Error);
        }

        accept();
      });
}

} // namespace soosh
