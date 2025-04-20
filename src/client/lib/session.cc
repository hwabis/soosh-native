#include "session.h"
#include <iostream>

namespace soosh {

Session::Session(ip::tcp::socket socket) : socket_(std::move(socket)) {}

void Session::Start() { listen(); }

void Session::SendMessage(const std::string &message) {
  auto self(shared_from_this());
  auto fullMessage = std::make_shared<std::string>(message + "\n");
  boost::asio::async_write(
      socket_, boost::asio::buffer(*fullMessage),
      [this, self, fullMessage](const boost::system::error_code &ec,
                                std::size_t) {
        if (ec) {
          std::cerr << "Error sending message: " << ec.message() << std::endl;
        }
      });
}

void Session::listen() {
  auto self(shared_from_this());
  boost::asio::async_read_until(
      socket_, streambuf_, '\n',
      [this, self](const boost::system::error_code &ec, std::size_t) {
        if (!ec) {
          std::istream input(&streambuf_);
          std::string message;
          std::getline(input, message);
          std::cout << "Server says: " << message << std::endl;
          listen();
        } else {
          handleReceiveError(ec);
        }
      });
}

void Session::handleReceiveError(const boost::system::error_code &ec) {
  if (ec == boost::asio::error::eof ||
      ec == boost::asio::error::connection_reset) {
    std::cout << "Server disconnected." << std::endl;
  } else {
    std::cerr << "Error while receiving message: " << ec.message() << std::endl;
  }

  if (socket_.is_open()) {
    socket_.shutdown(ip::tcp::socket::shutdown_both);
    socket_.close();
  }
}

} // namespace soosh
