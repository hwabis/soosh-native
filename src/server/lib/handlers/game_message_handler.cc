#pragma once

#include "handlers/game_message_handler.h"
#include "networking/client_session.h"
#include <optional>
#include <sstream>

namespace soosh {

GameMessageHandler::GameMessageHandler() : gameSession_() {}

void GameMessageHandler::OnMessageReceived(const soosh::ClientMessage &message,
                                           ClientSession &session) {
  const auto action = message.action();
  const std::string &payload = message.payload();

  ServerMessage response;

  switch (action) {
  case soosh::ActionType::Join: {
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
    int idx1 = -1;
    std::optional<int> idx2;
    std::stringstream ss(payload);
    ss >> idx1;
    if (ss >> *idx2) {
      // Successfully parsed a second index
    } else {
      idx2 = std::nullopt;
    }

    const std::string &playerName = session.GetPlayerName();
    if (!gameSession_.PlayCard(playerName, idx1, idx2)) {
      response.set_status(soosh::StatusType::Error);
      response.set_data(
          "Failed to play card(s). Invalid player or card index.");
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
