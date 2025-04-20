#pragma once

#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(ip::tcp::socket socket);
  void Start();
  void SendMessage(const std::string &message);

private:
  void listen();
  void handleReceiveError(const boost::system::error_code &ec);
  ip::tcp::socket socket_;
  boost::asio::streambuf streambuf_;
};

} // namespace soosh
