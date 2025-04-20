#include "client.h"
#include "session.h"
#include <iostream>

namespace soosh {

Client::Client(const std::string &serverAddress, unsigned short port)
    : ioContext_(), socket_(ioContext_),
      serverEndpoint_(ip::make_address(serverAddress), port) {}

void Client::Start() {
  try {
    socket_.connect(serverEndpoint_);
    if (!socket_.is_open()) {
      std::cerr << "Socket failed to open." << std::endl;
      return;
    }

    std::cout << "Connected to server at "
              << serverEndpoint_.address().to_string() << ":"
              << serverEndpoint_.port() << std::endl;

    std::shared_ptr<Session> session =
        std::make_shared<Session>(std::move(socket_));
    session->Start();

    std::jthread inputThread([session]() {
      std::string userInput;
      while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput == "quit") {
          std::cout << "Exiting client..." << std::endl;
          break;
        }

        session->SendMessage(userInput);
      }
    });

    ioContext_.run();
  } catch (const boost::system::system_error &e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
  }
}

} // namespace soosh
