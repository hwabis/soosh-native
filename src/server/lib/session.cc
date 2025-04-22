#include "session.h"
#include <iostream>

namespace soosh {

Session::Session(std::shared_ptr<ip::tcp::socket> socket,
                 GameMessageHandler handler)
    : socket_(std::move(socket)), handler_(std::move(handler)),
      timer_(socket_->get_executor()) {}

void Session::Start() {
  std::cout << "[INFO] Client connected.\n";
  listen();
  SendMessage("Hello from encapsulated server!\n");
}

void Session::SendMessage(const std::string &message) {
  auto self = shared_from_this();
  auto buffer = std::make_shared<std::string>(message);
  boost::asio::async_write(
      *socket_, boost::asio::buffer(*buffer),
      [this, self](const boost::system::error_code &ec, std::size_t) {
        if (!ec) {
          std::cout << "[INFO] Message sent to client.\n";
        } else {
          handleError(ec, "sending message");
        }
      });
}

void Session::listen() {
  auto self = shared_from_this();
  boost::asio::async_read_until(
      *socket_, buffer_, '\n',
      [this, self](const boost::system::error_code &ec, std::size_t) {
        if (!ec) {
          std::istream input(&buffer_);
          std::string message;
          std::getline(input, message);
          std::cout << "[CLIENT] " << message << '\n';

          handler_.OnMessageReceived(message, *self);

          listen();
        } else {
          handleError(ec, "receiving message");
        }
      });
}

void Session::handleError(const boost::system::error_code &ec,
                          const std::string &context) {
  if (ec == boost::asio::error::eof ||
      ec == boost::asio::error::connection_reset) {
    std::cout << "[INFO] Client disconnected.\n";
  } else {
    std::cerr << "[ERROR] " << context << ": " << ec.message() << '\n';
  }

  boost::system::error_code ignore;
  socket_->close(ignore);
}

} // namespace soosh
