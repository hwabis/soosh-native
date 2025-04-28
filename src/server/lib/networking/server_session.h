#pragma once

#include "game_message_handler.h"
#include "soosh.pb.h"
#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class ServerSession : public std::enable_shared_from_this<ServerSession> {
public:
  ServerSession(std::shared_ptr<ip::tcp::socket> socket,
                std::shared_ptr<GameMessageHandler> handler);
  void Start();
  void SendMessage(const soosh::ServerMessage &message);

  void SetPlayerName(const std::string &name) { playerName_ = name; }
  const std::string &GetPlayerName() const { return playerName_; }

private:
  void listen();
  void handleError(const boost::system::error_code &ec,
                   const std::string &context);
  std::shared_ptr<ip::tcp::socket> socket_;
  std::shared_ptr<GameMessageHandler> handler_;
  boost::asio::steady_timer timer_;
  boost::asio::streambuf buffer_;
  std::string playerName_;
};

} // namespace soosh
