#pragma once

#include "game_logic/models/card.h"
#include <string>
#include <utility>
#include <vector>

namespace soosh {

class Player {
public:
  explicit Player(std::string name) : name_(std::move(name)) {}

  [[nodiscard]] auto GetName() const -> const std::string & { return name_; }

  [[nodiscard]] auto GetHand() -> std::vector<Card> & { return hand_; }
  [[nodiscard]] auto GetHand() const -> const std::vector<Card> & {
    return hand_;
  }

  [[nodiscard]] auto GetInPlay() -> std::vector<Card> & { return inPlay_; }
  [[nodiscard]] auto GetInPlay() const -> const std::vector<Card> & {
    return inPlay_;
  }

  [[nodiscard]] auto GetEnqueuedCardsToPlay() -> std::vector<Card> & {
    return enqueuedCardsToPlay_;
  }
  [[nodiscard]] auto GetEnqueuedCardsToPlay() const
      -> const std::vector<Card> & {
    return enqueuedCardsToPlay_;
  }

  [[nodiscard]] auto GetEnqueuedCardsToHand() -> std::vector<Card> & {
    return enqueuedCardsToHand_;
  }
  [[nodiscard]] auto GetEnqueuedCardsToHand() const
      -> const std::vector<Card> & {
    return enqueuedCardsToHand_;
  }

  [[nodiscard]] auto HasFinishedTurn() const -> bool { return finishedTurn_; }
  void SetFinishedTurn(bool finished) { finishedTurn_ = finished; }

  [[nodiscard]] auto GetPoints() const -> int { return points_; }
  void SetPoints(int points) { points_ = points; }

private:
  std::string name_;
  std::vector<Card> hand_;
  std::vector<Card> inPlay_;
  std::vector<Card> enqueuedCardsToPlay_;
  std::vector<Card> enqueuedCardsToHand_;
  bool finishedTurn_ = false;
  int points_ = 0;
};

} // namespace soosh