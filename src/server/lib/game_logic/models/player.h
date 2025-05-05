#pragma once

#include "game_logic/models/card.h"
#include <string>
#include <vector>

namespace soosh {

class Player {
public:
  explicit Player(const std::string &name) : name_(name) {}

  const std::string &GetName() const { return name_; }

  std::vector<Card> &GetHand() { return hand_; }
  const std::vector<Card> &GetHand() const { return hand_; }

  std::vector<Card> &GetInPlay() { return inPlay_; }
  const std::vector<Card> &GetInPlay() const { return inPlay_; }

  std::vector<Card> &GetEnqueuedCardsToPlay() { return enqueuedCardsToPlay_; }
  const std::vector<Card> &GetEnqueuedCardsToPlay() const {
    return enqueuedCardsToPlay_;
  }

  std::vector<Card> &GetEnqueuedCardsToHand() { return enqueuedCardsToHand_; }
  const std::vector<Card> &GetEnqueuedCardsToHand() const {
    return enqueuedCardsToHand_;
  }

  bool HasFinishedTurn() const { return finishedTurn_; }
  void SetFinishedTurn(bool finished) { finishedTurn_ = finished; }

  int GetPoints() const { return points_; }
  void SetPoints(int points) { points_ = points; }

private:
  const std::string name_;
  std::vector<Card> hand_;
  std::vector<Card> inPlay_;
  std::vector<Card> enqueuedCardsToPlay_;
  std::vector<Card> enqueuedCardsToHand_;
  bool finishedTurn_ = false;
  int points_ = 0;
};

} // namespace soosh