#pragma once

#include "handlers/message_handler.h"
#include <boost/asio.hpp>

namespace ip = boost::asio::ip;

namespace soosh {

class Server {
public:
  explicit Server(unsigned short port);
  void Start();

private:
  void accept();

  std::shared_ptr<soosh::IMessageHandler> messageHandler_;
  boost::asio::io_context ioContext_;
  ip::tcp::acceptor acceptor_;
};

} // namespace soosh
