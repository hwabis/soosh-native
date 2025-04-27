#include "client.h"
#include "client_session.h"
#include "soosh.pb.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/system_error.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

namespace soosh {

Client::Client(const std::string &serverAddress, unsigned short port)
    : serverEndpoint_(ip::make_address(serverAddress), port) {}

void Client::Start() {
  try {
    ip::tcp::socket socket(ioContext_);
    socket.connect(serverEndpoint_);
    if (!socket.is_open()) {
      std::cerr << "Socket failed to open." << "\n";
      return;
    }

    std::cout << "Connected to server at "
              << serverEndpoint_.address().to_string() << ":"
              << serverEndpoint_.port() << "\n";

    auto session = std::make_shared<ClientSession>(std::move(socket));
    session->Start();

    const std::jthread ioThread([&] { ioContext_.run(); });

    std::cout << "Enter your player name: ";
    std::string playerName;
    std::getline(std::cin, playerName);
    soosh::ClientMessage joinMsg;
    joinMsg.set_action(soosh::ActionType::JOIN);
    joinMsg.set_payload(playerName);
    session->SendMessage(joinMsg);

    std::cout << "Available commands:\n";
    std::cout << "  start\n";
    std::cout << "  play <index1> <index2>\n";
    std::cout << "  quit\n\n";

    std::string userInput;
    while (true) {
      std::getline(std::cin, userInput);

      if (userInput == "quit") {
        std::cout << "Exiting client..." << "\n";
        ioContext_.stop();
        break;
      }

      soosh::ClientMessage msg;
      if (userInput == "start") {
        msg.set_action(soosh::ActionType::START);
        msg.set_payload("");
      } else if (userInput.starts_with("play ")) {
        msg.set_action(soosh::ActionType::PLAY);
        msg.set_payload(userInput.substr(5));
      } else {
        std::cerr << "Unknown command.\n";
        continue;
      }
      session->SendMessage(msg);
    }
  } catch (const boost::system::system_error &e) {
    std::cerr << "Connection failed: " << e.what() << "\n";
  }
}

} // namespace soosh
