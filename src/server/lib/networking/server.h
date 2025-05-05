#pragma once

#include "client_session.h"
#include "game_logic/game_session.h"
#include "handlers/message_handler.h"
#include <boost/asio.hpp>
#include <memory>

namespace ip = boost::asio::ip;

namespace soosh {

class Server : public std::enable_shared_from_this<Server> {
public:
  explicit Server(unsigned short port);
  void Start();
  const std::vector<std::shared_ptr<ClientSession>> &getSessions() const;

private:
  void accept();

  std::shared_ptr<soosh::IMessageHandler> messageHandler_;
  boost::asio::io_context ioContext_;
  ip::tcp::acceptor acceptor_;
  std::vector<std::shared_ptr<ClientSession>> sessions_;
  std::shared_ptr<GameSession> gameSession_;
};

} // namespace soosh
