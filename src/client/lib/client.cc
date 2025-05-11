#include "client.h"
#include "client_session.h"
#include "soosh.pb.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/system_error.hpp>
#include <memory>
#include <string>
#include <thread>

namespace soosh {

Client::Client(const std::string &serverAddress, unsigned short port)
    : serverEndpoint_(ip::make_address(serverAddress), port),
      ui_(std::make_shared<ClientUi>()) {}

void Client::Start() {
  try {
    ip::tcp::socket socket(ioContext_);
    socket.connect(serverEndpoint_);
    if (!socket.is_open()) {
      ui_->PrintError("Socket failed to open.");
      return;
    }

    ui_->PrintStatus(std::string("Connected to server at ") +
                     serverEndpoint_.address().to_string() + ":" +
                     std::to_string(serverEndpoint_.port()));

    auto session = std::make_shared<ClientSession>(std::move(socket), ui_);
    session->Start();

    const std::jthread ioThread([&] { ioContext_.run(); });

    std::string playerName = ui_->PromptInput("Enter your player name: ");
    ClientMessage joinMsg;
    joinMsg.set_action(ActionType::Join);
    joinMsg.mutable_join()->set_player_name(playerName);
    session->SendMessage(joinMsg);

    std::string helperMsg = "Available commands:\n"
                            "  start\n"
                            "  play <index1> <index2>\n"
                            "  quit\n\n";
    ui_->PrintStatus(helperMsg);

    std::string userInput;
    while (true) {
      std::getline(std::cin, userInput);
      std::stringstream ss(userInput);
      std::string command;
      ss >> command;

      if (userInput == "quit") {
        ioContext_.stop();
        break;
      }

      ClientMessage msg;
      if (userInput == "start") {
        msg.set_action(ActionType::Start);
      } else if (userInput.starts_with("play ")) {
        int index1 = -1, index2 = -1;
        if (!(ss >> index1)) {
          ui_->PrintError("Usage: play <index1> [index2]");
          continue;
        }
        msg.set_action(ActionType::Play);
        msg.mutable_play()->set_card_index1(index1);

        if (ss >> index2) {
          msg.mutable_play()->set_card_index2(index2);
        }
      } else {
        ui_->PrintError("Unknown command.");
        continue;
      }
      session->SendMessage(msg);
    }
  } catch (const boost::system::system_error &e) {
    ui_->PrintError(std::string("Connection failed: ") + e.what());
  }
}

} // namespace soosh
