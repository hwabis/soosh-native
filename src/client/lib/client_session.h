#pragma once

#include "soosh.pb.h"
#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
  explicit ClientSession(ip::tcp::socket socket);
  void Start();
  void SendMessage(const soosh::ClientMessage &message);

private:
  void listen();
  void handleReceiveError(const boost::system::error_code &ec);
  ip::tcp::socket socket_;
  boost::asio::streambuf streambuf_;
};

} // namespace soosh
