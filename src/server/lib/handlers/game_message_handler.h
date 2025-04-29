#pragma once

#include "game_logic/game_session.h"
#include "handlers/message_handler.h"
#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ServerSession;

class GameMessageHandler : public IMessageHandler {
public:
  GameMessageHandler();
  void OnMessageReceived(const soosh::ClientMessage &message,
                         ServerSession &session) override;

private:
  soosh::GameSession gameSession_;
};

} // namespace soosh
