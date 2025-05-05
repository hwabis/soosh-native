#include "handlers/game_message_handler.h"
#include "networking/client_session.h"
#include <optional>
#include <sstream>

namespace soosh {

GameMessageHandler::GameMessageHandler(std::shared_ptr<Server> server,
                                       std::shared_ptr<SooshSession> gameSession)
    : server_(server), gameSession_(gameSession) {}

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
        sendGameError(session,
                      "Already joined as: " + session->GetPlayerName());
      } else if (playerName.empty()) {
        sendGameError(session, "Player name cannot be empty.");
      } else if (!gameSession_->AddPlayer(playerName)) {
        sendGameError(session, "Failed to join.");
      } else {
        session->SetPlayerName(playerName);
        broadcastGameState();
      }
    } else {
      sendGameError(session, "Join action missing payload.");
    }
    break;
  }

  case soosh::ActionType::Start: {
    std::optional<std::string> error = gameSession_->StartGame();
    if (error.has_value()) {
      sendGameError(session, error.value());
    } else {
      broadcastGameState();
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
      if (!gameSession_->PlayCard(playerName, cardIndex1, cardIndex2)) {
        sendGameError(session, "Failed to play card(s).");
      } else {
        broadcastGameState();
      }
    } else {
      sendGameError(session, "Play action missing payload.");
    }
    break;
  }

  default:
    sendGameError(session, "Unknown action received.");
    break;
  }
}

void GameMessageHandler::OnClientDisconnected(
    std::shared_ptr<ClientSession> session) {
  if (session && !session->GetPlayerName().empty()) {
    gameSession_->RemovePlayer(session->GetPlayerName());
  }
}

void GameMessageHandler::broadcastGameState() {
  for (const auto &otherSession : server_->getSessions()) {
    ServerMessage updateMessage;
    updateMessage.set_status(soosh::StatusType::Update);
    updateMessage.set_data(gameSession_->SerializeGameState());
    otherSession->SendMessage(updateMessage);
  }
}

void GameMessageHandler::sendGameError(std::shared_ptr<ClientSession> session,
                                       const std::string &msg) {
  ServerMessage errorMessage;
  errorMessage.set_status(soosh::StatusType::Error);
  errorMessage.set_data(msg);
  session->SendMessage(errorMessage);
}

} // namespace soosh
