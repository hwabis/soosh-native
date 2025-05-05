#pragma once

#include "game_logic/soosh_session.h"
#include "handlers/message_handler.h"
#include "networking/server.h"
#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ClientSession;

class SooshMessageHandler : public IMessageHandler {
public:
  SooshMessageHandler(std::shared_ptr<Server> server,
                     std::shared_ptr<SooshSession> gameSession);
  void OnMessageReceived(const soosh::ClientMessage &message,
                         std::shared_ptr<ClientSession> session) override;
  void OnClientDisconnected(std::shared_ptr<ClientSession> session) override;

private:
  void broadcastGameState();
  void sendGameError(std::shared_ptr<ClientSession> session,
                     const std::string &msg);
  std::shared_ptr<Server> server_;
  std::shared_ptr<SooshSession> gameSession_;
};

} // namespace soosh
