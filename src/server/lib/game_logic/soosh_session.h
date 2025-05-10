#pragma once

#include "game_logic/models/card.h"
#include "game_logic/models/player.h"
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <vector>

namespace soosh {

enum class GameStage : uint8_t { Waiting, Playing, Finished };

class SooshSession {
public:
  SooshSession();

  auto AddPlayer(const std::string &playerName) -> bool;
  auto RemovePlayer(const std::string &playerName) -> bool;
  auto StartGame() -> std::optional<std::string>;
  auto PlayCard(const std::string &playerName, int cardIndex1,
                std::optional<int> cardIndex2) -> bool;

  [[nodiscard]] auto GetGameStage() const -> GameStage;
  [[nodiscard]] auto GetPlayers() const
      -> const std::vector<std::unique_ptr<Player>> &;

  // todo this should actually be a game object and not a string
  [[nodiscard]] auto SerializeGameState() const -> std::string;

private:
  void resetGame();
  void resetRound();
  void distributeCards();
  void rotateHands();
  [[nodiscard]] auto checkRoundEnd() -> bool;
  [[nodiscard]] auto checkGameEnd() const -> bool;
  void advanceRound();
  void initDeck();

  std::vector<std::unique_ptr<Player>> players_;
  std::stack<Card> deck_;
  GameStage gameStage_;
  int numberOfRoundsCompleted_;
};

} // namespace soosh
