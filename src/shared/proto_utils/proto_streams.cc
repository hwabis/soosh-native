#include "proto_streams.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace soosh {
namespace utils {

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

} // namespace utils
} // namespace soosh
