#pragma once

#include "handlers/message_handler.h"
#include "soosh.pb.h"
#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
  ClientSession(std::shared_ptr<ip::tcp::socket> socket,
                std::shared_ptr<IMessageHandler> handler);
  void Start();
  void SendMessage(const soosh::ServerMessage &message) const;

  void SetPlayerName(const std::string &name) { playerName_ = name; }
  [[nodiscard]] auto GetPlayerName() const -> const std::string & {
    return playerName_;
  }

private:
  void listen();
  void handleError(const boost::system::error_code &ec,
                   const std::string &context);
  std::shared_ptr<ip::tcp::socket> socket_;
  std::shared_ptr<IMessageHandler> handler_;
  boost::asio::steady_timer timer_;
  boost::asio::streambuf buffer_;
  std::string playerName_; // todo ... optional (for at the start)
};

} // namespace soosh
