#include "game_message_handler.h"
#include "session.h"
#include <iostream>

namespace soosh {

void GameMessageHandler::OnMessageReceived(const std::string &message,
                                           Session &session) {
  std::cout << "[GAME LOGIC] Processing message: " << message << '\n';
  session.SendMessage("Game Logic Acknowledged: " + message + "\n");
}

} // namespace soosh
