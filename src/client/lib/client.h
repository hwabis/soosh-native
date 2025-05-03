#pragma once

#include "client_ui.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class Client {
public:
  Client(const std::string &serverAddress, unsigned short port);
  void Start();

private:
  boost::asio::io_context ioContext_;
  ip::tcp::endpoint serverEndpoint_;
  std::shared_ptr<ClientUi> ui_;
};

} // namespace soosh
