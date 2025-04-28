#pragma once

#include "game_logic/game_session.h"
#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ServerSession;

class GameMessageHandler {
public:
  GameMessageHandler();
  void OnMessageReceived(const soosh::ClientMessage &message,
                         ServerSession &session);

private:
  soosh::GameSession gameSession_;
};

} // namespace soosh
