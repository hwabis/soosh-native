#pragma once

#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class Client {
public:
  Client(const std::string &serverAddress, unsigned short port);
  void Start();

private:
  boost::asio::io_context ioContext_;
  ip::tcp::socket socket_;
  ip::tcp::endpoint serverEndpoint_;
};

} // namespace soosh
