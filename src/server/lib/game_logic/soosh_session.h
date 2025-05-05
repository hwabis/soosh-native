#pragma once

#include "models/card.h"
#include "models/player.h"
#include <memory>
#include <optional> // For std::optional
#include <stack>
#include <string>
#include <vector>

namespace soosh {

enum class GameStage { Waiting, Playing, Finished };

class SooshSession {
public:
  SooshSession();

  bool AddPlayer(const std::string &playerName);
  bool RemovePlayer(const std::string &playerName);
  std::optional<std::string> StartGame();
  bool PlayCard(const std::string &playerName, int cardIndex1,
                std::optional<int> cardIndex2);

  GameStage GetGameStage() const;
  const std::vector<std::unique_ptr<Player>> &GetPlayers() const;

  std::string SerializeGameState() const;

private:
  void resetGame();
  void resetRound();
  void distributeCards();
  void rotateHands();
  bool checkRoundEnd();
  bool checkGameEnd();
  void advanceRound();
  void initDeck();

  std::vector<std::unique_ptr<Player>> players_;
  std::stack<Card> deck_;
  GameStage gameStage_;
  int numberOfRoundsCompleted_;
};

} // namespace soosh
