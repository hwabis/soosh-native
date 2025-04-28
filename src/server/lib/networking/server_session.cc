#include "networking/server_session.h"
#include "protobuf_stream_utils.h"
#include <iostream>

namespace soosh {

ServerSession::ServerSession(std::shared_ptr<ip::tcp::socket> socket,
                             std::shared_ptr<GameMessageHandler> handler)
    : socket_(std::move(socket)), handler_(std::move(handler)),
      timer_(socket_->get_executor()) {}

void ServerSession::Start() {
  std::cout << "[INFO] Client connected.\n";
  listen();

  soosh::ServerMessage msg;
  msg.set_status("ok");
  msg.set_data("Hello from encapsulated server!");
  SendMessage(msg);
}

void ServerSession::SendMessage(const soosh::ServerMessage &message) {
  auto self = shared_from_this();

  soosh::AsyncWriteProtobuf(
      *socket_, message, [self](const boost::system::error_code &ec) {
        if (ec) {
          std::cerr << "Error sending message: " << ec.message() << '\n';
        }
      });
}

void ServerSession::listen() {
  auto self = shared_from_this();

  soosh::AsyncReadProtobuf<soosh::ClientMessage>(
      *socket_, [this, self](const boost::system::error_code &ec,
                             std::shared_ptr<soosh::ClientMessage> msg) {
        if (ec) {
          handleError(ec, "receiving message");
          return;
        }

        if (msg) {
          std::cout << playerName_ << " ACTION: " << msg->action()
                    << ", payload: " << msg->payload() << '\n';

          handler_->OnMessageReceived(*msg, *self);

          listen();
        } else {
          handleError(boost::asio::error::operation_aborted,
                      "null message received");
        }
      });
}

void ServerSession::handleError(const boost::system::error_code &ec,
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
