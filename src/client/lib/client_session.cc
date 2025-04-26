#include "client_session.h"
#include "protobuf_stream_utils.h"
#include <iostream>

namespace soosh {

ClientSession::ClientSession(ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

void ClientSession::Start() { listen(); }

void ClientSession::SendMessage(const soosh::ClientMessage &message) {
  auto self = shared_from_this();

  soosh::AsyncWriteProtobuf(
      socket_, message, [self](const boost::system::error_code &ec) {
        if (ec) {
          std::cerr << "Error sending message: " << ec.message() << '\n';
        }
      });
}

void ClientSession::listen() {
  auto self = shared_from_this();

  soosh::AsyncReadProtobuf<soosh::ServerMessage>(
      socket_, [this, self](const boost::system::error_code &ec,
                            std::shared_ptr<soosh::ServerMessage> msg) {
        if (ec) {
          handleReceiveError(ec);
          return;
        }

        if (msg) {
          std::cout << "Server status: " << msg->status()
                    << ", data: " << msg->data() << '\n';
          listen();
        } else {
          handleReceiveError(boost::asio::error::operation_aborted);
        }
      });
}

void ClientSession::handleReceiveError(const boost::system::error_code &ec) {
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
