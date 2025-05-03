#pragma once

#include "client_ui.h"
#include "soosh.pb.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
  explicit ClientSession(ip::tcp::socket socket, std::shared_ptr<ClientUi> ui);
  void Start();
  void SendMessage(const soosh::ClientMessage &message);

private:
  void listen();
  void handleReceiveError(const boost::system::error_code &ec);
  ip::tcp::socket socket_;
  boost::asio::streambuf streambuf_;
  std::shared_ptr<ClientUi> ui_;
};

} // namespace soosh
