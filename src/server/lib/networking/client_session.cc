#include "networking/client_session.h"
#include "protobuf_stream_utils.h"
#include "utils/logger.h"

namespace soosh {

ClientSession::ClientSession(std::shared_ptr<ip::tcp::socket> socket,
                             std::shared_ptr<IMessageHandler> handler)
    : socket_(std::move(socket)), handler_(std::move(handler)),
      timer_(socket_->get_executor()) {}

void ClientSession::Start() {
  Logger::Log("Client connected.");
  listen();

  soosh::ServerMessage msg;
  msg.set_status("ok");
  msg.set_data("Hello from encapsulated server!");
  SendMessage(msg);
}

void ClientSession::SendMessage(const soosh::ServerMessage &message) {
  auto self = shared_from_this();

  soosh::AsyncWriteProtobuf(
      *socket_, message, [self](const boost::system::error_code &ec) {
        if (ec) {
          Logger::Log("Error sending message: " + ec.message(),
                      Logger::Level::Error);
        }
      });
}

void ClientSession::listen() {
  auto self = shared_from_this();

  soosh::AsyncReadProtobuf<soosh::ClientMessage>(
      *socket_, [this, self](const boost::system::error_code &ec,
                             std::shared_ptr<soosh::ClientMessage> msg) {
        if (ec) {
          handleError(ec, "receiving message");
          return;
        }

        if (msg) {
          Logger::Log(playerName_ + " ACTION: " +
                      std::to_string(static_cast<int>(msg->action())) +
                      ", payload: " + std::string(msg->payload())); // todo

          handler_->OnMessageReceived(*msg, *self);

          listen();
        } else {
          handleError(boost::asio::error::operation_aborted,
                      "null message received");
        }
      });
}

void ClientSession::handleError(const boost::system::error_code &ec,
                                const std::string &context) {
  if (ec == boost::asio::error::eof ||
      ec == boost::asio::error::connection_reset) {
    Logger::Log("Client disconnected.");
  } else {
    Logger::Log(context + ": " + ec.message(), Logger::Level::Error);
  }

  boost::system::error_code ignore;
  socket_->close(ignore);
}

} // namespace soosh
