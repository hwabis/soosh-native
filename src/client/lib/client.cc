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

    std::string userInput;
    while (true) {
      std::cout << "> ";
      std::getline(std::cin, userInput);

      if (userInput == "quit") {
        std::cout << "Exiting client..." << "\n";
        ioContext_.stop();
        break;
      }

      auto spacePos = userInput.find(' ');
      std::string action = userInput.substr(0, spacePos);
      std::string payload =
          (spacePos == std::string::npos) ? "" : userInput.substr(spacePos + 1);

      soosh::ClientMessage msg;
      msg.set_action(action);
      msg.set_payload(payload);
      session->SendMessage(msg);
    }
  } catch (const boost::system::system_error &e) {
    std::cerr << "Connection failed: " << e.what() << "\n";
  }
}

} // namespace soosh
