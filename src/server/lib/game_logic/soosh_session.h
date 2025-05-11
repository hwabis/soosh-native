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
  auto Start() -> std::optional<std::string>;
  auto PlayCard(const std::string &playerName, int cardIndex1,
                std::optional<int> cardIndex2) -> bool;

  [[nodiscard]] auto GetGameStage() const -> GameStage;
  [[nodiscard]] auto GetPlayers() const
      -> const std::vector<std::unique_ptr<Player>> &;

  // todo this should actually be a game object and not a string
  [[nodiscard]] auto SerializeGameState() const -> std::string;
  [[nodiscard]] auto SerializeHand(const std::string &playerName) const
      -> std::string;

private:
  void resetGame();
  void startRound();
  void onTurnEnd();
  void rotateHands();
  void resetDeck();
  [[nodiscard]] auto hasChopsticksInPlay(const Player &player) const -> bool;
  void clearHandExceptPuddings(Player &player);

  std::vector<std::unique_ptr<Player>> players_;
  std::stack<Card> deck_;
  GameStage gameStage_;
  int numberOfRoundsCompleted_;

  static constexpr int playerLimit_ = 5;
  static constexpr int maxNumberOfRounds_ = 3;
};

} // namespace soosh
