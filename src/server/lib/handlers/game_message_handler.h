#pragma once

#include "game_logic/game_session.h"
#include "handlers/message_handler.h"
#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ClientSession;

class GameMessageHandler : public IMessageHandler {
public:
  GameMessageHandler();
  void OnMessageReceived(const soosh::ClientMessage &message,
                         std::shared_ptr<ClientSession> session) override;
  void OnClientDisconnected(std::shared_ptr<ClientSession> session) override;

private:
  soosh::GameSession gameSession_;
};

} // namespace soosh
