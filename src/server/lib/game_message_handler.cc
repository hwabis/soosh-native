#include "game_message_handler.h"
#include "networking/server_session.h"

namespace soosh {

GameMessageHandler::GameMessageHandler() : gameSession_() {}

void GameMessageHandler::OnMessageReceived(const soosh::ClientMessage &message,
                                           ServerSession &session) {
  const std::string &action = message.action();
  const std::string &payload = message.payload();

  if (action == "join") {
    gameSession_.AddPlayer(payload);
  } else if (action == "start") {
    std::string error;
    if (!gameSession_.StartGame(error)) {
      ServerMessage errorMsg;
      errorMsg.set_status("error");
      errorMsg.set_data(error);
      session.SendMessage(errorMsg);
      return;
    }
  } else if (action == "play") {
    // Expected format: "index1 index2"
    int idx1 = -1, idx2 = -1;

#pragma warning(push)
#pragma warning(disable : 4996)
    int numScanned = sscanf(payload.c_str(), "%d %d", &idx1, &idx2);
    (void)numScanned;
#pragma warning(pop)

    std::string playerName = "TODO";

    std::string error;
    if (!gameSession_.PlayCard(playerName, idx1, idx2, error)) {
      ServerMessage errorMsg;
      errorMsg.set_status("error");
      errorMsg.set_data(error);
      session.SendMessage(errorMsg);
      return;
    }
  } else {
    ServerMessage errorMsg;
    errorMsg.set_status("error");
    errorMsg.set_data("Unknown action: " + action);
    session.SendMessage(errorMsg);
    return;
  }

  ServerMessage stateMsg;
  stateMsg.set_status("update");
  stateMsg.set_data(gameSession_.SerializeGameState());
  session.SendMessage(stateMsg);
}

} // namespace soosh
