#include "handlers/game_message_handler.h"
#include "networking/client_session.h"
#include <optional>
#include <sstream>

namespace soosh {

GameMessageHandler::GameMessageHandler() : gameSession_() {}

void GameMessageHandler::OnMessageReceived(
    const soosh::ClientMessage &message,
    std::shared_ptr<ClientSession> session) {
  const auto action = message.action();
  ServerMessage response;

  switch (action) {
  case soosh::ActionType::Join: {
    if (message.has_join()) {
      const std::string &playerName = message.join().player_name();
      if (!session->GetPlayerName().empty()) {
        response.set_status(soosh::StatusType::Error);
        response.set_data("Already joined as: " + session->GetPlayerName());
      } else if (playerName.empty()) {
        response.set_status(soosh::StatusType::Error);
        response.set_data("Player name cannot be empty.");
      } else if (!gameSession_.AddPlayer(playerName)) {
        response.set_status(soosh::StatusType::Error);
        response.set_data("Failed to join.");
      } else {
        session->SetPlayerName(playerName);
        response.set_status(soosh::StatusType::Update);
        response.set_data(gameSession_.SerializeGameState());
      }
    } else {
      response.set_status(soosh::StatusType::Error);
      response.set_data("Join action missing payload.");
    }
    break;
  }

  case soosh::ActionType::Start: {
    std::optional<std::string> error = gameSession_.StartGame();
    if (error.has_value()) {
      response.set_status(soosh::StatusType::Error);
      response.set_data(error.value());
    } else {
      response.set_status(soosh::StatusType::Update);
      response.set_data(gameSession_.SerializeGameState());
    }
    break;
  }

  case soosh::ActionType::Play: {
    if (message.has_play()) {
      int cardIndex1 = message.play().card_index1();
      std::optional<int> cardIndex2 =
          message.play().has_card_index2()
              ? std::make_optional(message.play().card_index2())
              : std::nullopt;

      const std::string &playerName = session->GetPlayerName();
      if (!gameSession_.PlayCard(playerName, cardIndex1, cardIndex2)) {
        response.set_status(soosh::StatusType::Error);
        response.set_data("Failed to play card(s).");
      } else {
        response.set_status(soosh::StatusType::Update);
        response.set_data(gameSession_.SerializeGameState());
      }
    } else {
      response.set_status(soosh::StatusType::Error);
      response.set_data("Play action missing payload.");
    }
    break;
  }

  default:
    response.set_status(soosh::StatusType::Error);
    response.set_data("Unknown action received.");
    break;
  }

  session->SendMessage(response);
}

void GameMessageHandler::OnClientDisconnected(
    std::shared_ptr<ClientSession> session) {
  if (session && !session->GetPlayerName().empty()) {
    gameSession_.RemovePlayer(session->GetPlayerName());
  }
}

} // namespace soosh
