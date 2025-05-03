#include "handlers/game_message_handler.h"
#include "networking/client_session.h"

namespace soosh {

GameMessageHandler::GameMessageHandler() : gameSession_() {}

void GameMessageHandler::OnMessageReceived(const soosh::ClientMessage &message,
                                           ClientSession &session) {
  const auto action = message.action();
  const std::string &payload = message.payload();

  ServerMessage response;

  switch (action) {
  case soosh::ActionType::JOIN: {
    if (!session.GetPlayerName().empty()) {
      response.set_status(soosh::StatusType::Error);
      response.set_data("Already joined as: " + session.GetPlayerName());
    } else if (payload.empty()) {
      response.set_status(soosh::StatusType::Error);
      response.set_data("Player name cannot be empty.");
    } else if (!gameSession_.AddPlayer(payload)) {
      response.set_status(soosh::StatusType::Error);
      response.set_data("Failed to join. Name may already exist.");
    } else {
      session.SetPlayerName(payload);
      response.set_status(soosh::StatusType::Update);
      response.set_data(gameSession_.SerializeGameState());
    }
    break;
  }

  case soosh::ActionType::START: {
    std::string error;
    if (!gameSession_.StartGame(error)) {
      response.set_status(soosh::StatusType::Error);
      response.set_data(error);
    } else {
      response.set_status(soosh::StatusType::Update);
      response.set_data(gameSession_.SerializeGameState());
    }
    break;
  }

  case soosh::ActionType::PLAY: {
    int idx1 = -1, idx2 = -1;
#pragma warning(push)
#pragma warning(disable : 4996)
    sscanf(payload.c_str(), "%d %d", &idx1, &idx2);
#pragma warning(pop)

    const std::string &playerName = session.GetPlayerName();
    std::string error;
    if (!gameSession_.PlayCard(playerName, idx1, idx2, error)) {
      response.set_status(soosh::StatusType::Error);
      response.set_data(error);
    } else {
      response.set_status(soosh::StatusType::Update);
      response.set_data(gameSession_.SerializeGameState());
    }
    break;
  }

  default:
    response.set_status(soosh::StatusType::Error);
    response.set_data("Unknown action received.");
    break;
  }

  session.SendMessage(response);
}

} // namespace soosh
