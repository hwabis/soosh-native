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

  soosh::ServerMessage msg;
  msg.set_status("ok");
  msg.set_data("Hello from encapsulated server!");
  SendMessage(msg);
}

void Session::SendMessage(const soosh::ServerMessage &message) {
  auto self = shared_from_this();

  auto serialized = std::make_shared<std::string>();
  if (!message.SerializeToString(serialized.get())) {
    std::cerr << "Failed to serialize ServerMessage\n";
    return;
  }

  uint32_t size = static_cast<uint32_t>(serialized->size());
  auto sizeBuf = std::make_shared<std::array<char, 4>>();
  std::memcpy(sizeBuf->data(), &size, 4);

  std::vector<boost::asio::const_buffer> buffers = {
      boost::asio::buffer(*sizeBuf), boost::asio::buffer(*serialized)};

  boost::asio::async_write(
      *socket_, buffers,
      [this, self, sizeBuf, serialized](const boost::system::error_code &ec,
                                        std::size_t) {
        if (ec) {
          handleError(ec, "sending message");
        } else {
          std::cout << "[INFO] Message sent to client.\n";
        }
      });
}

void Session::listen() {
  auto self = shared_from_this();
  auto sizeBuf = std::make_shared<std::array<char, 4>>();

  boost::asio::async_read(
      *socket_, boost::asio::buffer(*sizeBuf),
      [this, self, sizeBuf](const boost::system::error_code &ec, std::size_t) {
        if (ec) {
          handleError(ec, "receiving size");
          return;
        }

        uint32_t msgSize;
        std::memcpy(&msgSize, sizeBuf->data(), 4);

        auto msgBuf = std::make_shared<std::vector<char>>(msgSize);
        boost::asio::async_read(
            *socket_, boost::asio::buffer(*msgBuf),
            [this, self, msgBuf](const boost::system::error_code &ec,
                                 std::size_t) {
              if (ec) {
                handleError(ec, "receiving message body");
                return;
              }

              soosh::ClientMessage msg;
              if (!msg.ParseFromArray(msgBuf->data(),
                                      static_cast<int>(msgBuf->size()))) {
                std::cerr << "Failed to parse ClientMessage\n";
                return;
              }

              std::cout << "[CLIENT ACTION] " << msg.action()
                        << ", payload: " << msg.payload() << '\n';

              handler_.OnMessageReceived(msg, *self);

              listen();
            });
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
