#pragma once

#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh {

class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(ip::tcp::socket socket);
  void Start();

private:
  void readMessage();
  void sendMessage(const std::string &message);
  void handleError(const boost::system::error_code &ec,
                   const std::string &context);
  ip::tcp::socket socket_;
  boost::asio::steady_timer timer_;
  boost::asio::streambuf buffer_;
};

} // namespace soosh
