#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <google/protobuf/message.h>
#include <memory>

namespace soosh {

void AsyncWriteProtobuf(
    boost::asio::ip::tcp::socket &socket,
    const google::protobuf::Message &message,
    std::function<void(const boost::system::error_code &)> onComplete) {

  auto serialized = std::make_shared<std::string>();
  if (!message.SerializeToString(serialized.get())) {
    std::cerr << "Failed to serialize protobuf message" << std::endl;
    onComplete(boost::asio::error::operation_aborted);
    return;
  }

  uint32_t size = static_cast<uint32_t>(serialized->size());
  auto sizeBuf = std::make_shared<std::array<char, 4>>();
  std::memcpy(sizeBuf->data(), &size, 4);

  std::vector<boost::asio::const_buffer> buffers = {
      boost::asio::buffer(*sizeBuf), boost::asio::buffer(*serialized)};

  boost::asio::async_write(
      socket, buffers,
      [sizeBuf, serialized, onComplete](const boost::system::error_code &ec,
                                        std::size_t) { onComplete(ec); });
}

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
          std::cerr << "Protobuf message size too large: " << msgSize
                    << std::endl;
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
                std::cerr << "Failed to parse protobuf message" << std::endl;
                onMessage(boost::asio::error::operation_aborted, nullptr);
                return;
              }

              onMessage({}, message);
            });
      });
}

} // namespace soosh
