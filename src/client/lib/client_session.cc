#include "client_session.h"
#include "client_ui.h"
#include "protobuf_stream_utils.h"

namespace soosh {

ClientSession::ClientSession(ip::tcp::socket socket,
                             std::shared_ptr<ClientUi> ui)
    : socket_(std::move(socket)), ui_(std::move(ui)) {}

void ClientSession::Start() { listen(); }

void ClientSession::SendMessage(const soosh::ClientMessage &message) {
  auto self = shared_from_this();

  soosh::AsyncWriteProtobuf(
      socket_, message, [self, this](const boost::system::error_code &ec) {
        if (ec) {
          ui_->PrintError("Error sending message: " + ec.message());
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
          ui_->PrintStatus(std::string("Server status: ") +
                           std::to_string(msg->status()) +
                           std::string(", data: ") + msg->data());
          listen();
        } else {
          handleReceiveError(boost::asio::error::operation_aborted);
        }
      });
}

void ClientSession::handleReceiveError(const boost::system::error_code &ec) {
  if (ec == boost::asio::error::eof ||
      ec == boost::asio::error::connection_reset) {
    ui_->PrintStatus("Server disconnected.");
  } else {
    ui_->PrintError("Error while receiving message:" + ec.message());
  }

  if (socket_.is_open()) {
    socket_.shutdown(ip::tcp::socket::shutdown_both);
    socket_.close();
  }
}

} // namespace soosh
