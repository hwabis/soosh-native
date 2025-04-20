#pragma once

#include <boost/asio.hpp>

namespace ip = boost::asio::ip;

namespace soosh {

class Server {
public:
  explicit Server(unsigned short port);
  void Start();

private:
  void accept();

  boost::asio::io_context ioContext_;
  ip::tcp::acceptor acceptor_;
};

} // namespace soosh
