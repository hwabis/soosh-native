#pragma once

#include <boost/asio.hpp>
#include <google/protobuf/message.h>

namespace soosh {
namespace utils {

void AsyncWriteProtobuf(
    boost::asio::ip::tcp::socket &socket,
    const google::protobuf::Message &message,
    const std::function<void(const boost::system::error_code &)> &onComplete);

template <typename MessageType>
void AsyncReadProtobuf(boost::asio::ip::tcp::socket &socket,
                       std::function<void(const boost::system::error_code &,
                                          std::shared_ptr<MessageType>)>
                           onMessage) {
  auto sizeBuf = std::make_shared<std::array<char, 4>>();

  boost::asio::async_read(
      socket, boost::asio::buffer(*sizeBuf),
      [sizeBuf, &socket, onMessage](const boost::system::error_code &ec,
                                    std::size_t) {
        if (ec) {
          onMessage(ec, nullptr);
          return;
        }

        uint32_t msgSize = 0;
        std::memcpy(&msgSize, sizeBuf->data(), 4);

        if (msgSize > 10 * 1024 * 1024) {
          std::cerr << "Protobuf message size too large: " << msgSize << '\n';
          onMessage(boost::asio::error::message_size, nullptr);
          return;
        }

        auto msgBuf = std::make_shared<std::vector<char>>(msgSize);

        if (msgSize == 0) {
          auto message = std::make_shared<MessageType>();
          if (message->ParseFromString("")) {
            onMessage({}, message);
          } else {
            onMessage(boost::asio::error::operation_aborted, nullptr);
          }
          return;
        }

        boost::asio::async_read(
            socket, boost::asio::buffer(*msgBuf),
            [msgBuf, onMessage](const boost::system::error_code &ec,
                                std::size_t bytes_read) {
              if (ec) {
                onMessage(ec, nullptr);
                return;
              }

              auto message = std::make_shared<MessageType>();
              if (!message->ParseFromArray(msgBuf->data(),
                                           static_cast<int>(bytes_read))) {
                std::cerr << "Failed to parse protobuf message" << '\n';
                onMessage(boost::asio::error::operation_aborted, nullptr);
                return;
              }

              onMessage({}, message);
            });
      });
}

} // namespace utils
} // namespace soosh
