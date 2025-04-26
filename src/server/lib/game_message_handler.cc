#include "game_message_handler.h"
#include "server_session.h"
#include <iostream>

namespace soosh {

void GameMessageHandler::OnMessageReceived(const soosh::ClientMessage &message,
                                           ServerSession &session) {
  std::cout << "[GAME LOGIC] " << message.action()
            << ", payload: " << message.payload() << '\n';
}

} // namespace soosh
