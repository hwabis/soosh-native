#pragma once

#include "game_message_handler.h"
#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(std::shared_ptr<ip::tcp::socket> socket,
          std::unique_ptr<GameMessageHandler> handler);
  void Start();
  void SendMessage(const std::string &message);

private:
  void listen();
  void handleError(boost::system::error_code ec, std::string context);
  std::shared_ptr<ip::tcp::socket> socket_;
  std::unique_ptr<GameMessageHandler> handler_;
  boost::asio::steady_timer timer_;
  boost::asio::streambuf buffer_;
};

} // namespace soosh
