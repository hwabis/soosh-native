#pragma once

#include "game_logic/game_session.h"
#include "handlers/message_handler.h"
#include "networking/server.h"
#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ClientSession;

class GameMessageHandler : public IMessageHandler {
public:
  GameMessageHandler(std::shared_ptr<Server> server);
  void OnMessageReceived(const soosh::ClientMessage &message,
                         std::shared_ptr<ClientSession> session) override;
  void OnClientDisconnected(std::shared_ptr<ClientSession> session) override;

private:
  void broadcastGameState();
  void sendGameError(const std::string &msg);

  // todo this separation of concerns isn't perfect 😬
  std::shared_ptr<Server> server_;
  soosh::GameSession gameSession_;
};

} // namespace soosh
