#include "client_session.h"
#include "client_ui.h"
#include "proto_enum_conversions.h"
#include "proto_streams.h"

namespace soosh {

ClientSession::ClientSession(ip::tcp::socket socket,
                             std::shared_ptr<ClientUi> ui)
    : socket_(std::move(socket)), ui_(std::move(ui)) {}

void ClientSession::Start() { listen(); }

void ClientSession::SendMessage(const ClientMessage &message) {
  auto self = shared_from_this();

  soosh::utils::AsyncWriteProtobuf(
      socket_, message, [self, this](const boost::system::error_code &ec) {
        if (ec) {
          ui_->PrintError("Error sending message: " + ec.message());
        }
      });
}

void ClientSession::listen() {
  auto self = shared_from_this();

  soosh::utils::AsyncReadProtobuf<ServerMessage>(
      socket_, [this, self](const boost::system::error_code &ec,
                            const std::shared_ptr<ServerMessage> &msg) {
        if (ec) {
          handleReceiveError(ec);
          return;
        }

        if (msg) {
          ui_->PrintStatus(std::string("Server status: ") +
                           soosh::utils::StatusTypeToString(msg->status()) +
                           std::string("\n") + msg->data() +
                           "=========================");
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
