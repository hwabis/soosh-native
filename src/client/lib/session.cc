#include "session.h"
#include <iostream>

namespace soosh {

Session::Session(ip::tcp::socket socket) : socket_(std::move(socket)) {}

void Session::Start() { listen(); }

void Session::SendMessage(const soosh::ClientMessage &message) {
  auto self = shared_from_this();

  auto serialized = std::make_shared<std::string>();
  if (!message.SerializeToString(serialized.get())) {
    std::cerr << "Failed to serialize ClientMessage\n";
    return;
  }

  uint32_t size = static_cast<uint32_t>(serialized->size());
  auto sizeBuf = std::make_shared<std::array<char, 4>>();
  std::memcpy(sizeBuf->data(), &size, 4);

  std::vector<boost::asio::const_buffer> buffers = {
      boost::asio::buffer(*sizeBuf), boost::asio::buffer(*serialized)};

  boost::asio::async_write(
      socket_, buffers,
      [self, sizeBuf, serialized](const boost::system::error_code &ec,
                                  std::size_t) {
        if (ec) {
          std::cerr << "Error sending message: " << ec.message() << '\n';
        }
      });
}

void Session::listen() {
  auto self = shared_from_this();
  auto sizeBuf = std::make_shared<std::array<char, 4>>();

  boost::asio::async_read(
      socket_, boost::asio::buffer(*sizeBuf),
      [this, self, sizeBuf](const boost::system::error_code &ec, std::size_t) {
        if (ec) {
          handleReceiveError(ec);
          return;
        }

        uint32_t msgSize;
        std::memcpy(&msgSize, sizeBuf->data(), 4);

        auto msgBuf = std::make_shared<std::vector<char>>(msgSize);
        boost::asio::async_read(
            socket_, boost::asio::buffer(*msgBuf),
            [this, self, msgBuf](const boost::system::error_code &ec,
                                 std::size_t) {
              if (ec) {
                handleReceiveError(ec);
                return;
              }

              soosh::ServerMessage msg;
              if (!msg.ParseFromArray(msgBuf->data(),
                                      static_cast<int>(msgBuf->size()))) {
                std::cerr << "Failed to parse ServerMessage\n";
                return;
              }

              std::cout << "Server status: " << msg.status()
                        << ", data: " << msg.data() << '\n';

              listen();
            });
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
