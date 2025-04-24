#include "client.h"
#include "session.h"
#include <iostream>

namespace soosh {

Client::Client(const std::string &serverAddress, unsigned short port)
    : ioContext_(), serverEndpoint_(ip::make_address(serverAddress), port) {}

void Client::Start() {
  try {
    ip::tcp::socket socket(ioContext_);
    socket.connect(serverEndpoint_);
    if (!socket.is_open()) {
      std::cerr << "Socket failed to open." << std::endl;
      return;
    }

    std::cout << "Connected to server at "
              << serverEndpoint_.address().to_string() << ":"
              << serverEndpoint_.port() << std::endl;

    auto session = std::make_shared<Session>(std::move(socket));
    session->Start();

    std::jthread ioThread([&] { ioContext_.run(); });

    std::string userInput;
    while (true) {
      std::cout << "> ";
      std::getline(std::cin, userInput);

      if (userInput == "quit") {
        std::cout << "Exiting client..." << std::endl;
        ioContext_.stop();
        break;
      }

      soosh::ClientMessage msg;
      msg.set_action("join");
      msg.set_payload(userInput);
      session->SendMessage(msg);
    }
  } catch (const boost::system::system_error &e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
  }
}

} // namespace soosh
