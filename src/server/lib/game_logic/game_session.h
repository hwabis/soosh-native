#pragma once

#include "models/card.h"
#include "models/player.h"
#include <stack>
#include <string>
#include <vector>

namespace soosh {

enum class GameStage { Waiting, Playing, Finished };

class GameSession {
public:
  GameSession();

  bool AddPlayer(const std::string &playerName);
  bool StartGame(std::string &error);
  bool PlayCard(const std::string &playerName, int cardIndex1, int cardIndex2,
                std::string &error);

  GameStage GetGameStage() const;
  const std::vector<Player> &GetPlayers() const;

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

  std::vector<Player> players_;
  std::stack<Card> deck_;
  GameStage gameStage_;
  int numberOfRoundsCompleted_;
};

} // namespace soosh
