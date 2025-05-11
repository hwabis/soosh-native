#include "game_logic/soosh_session.h"
#include "game_logic/scoring.h"
#include <algorithm>
#include <optional>
#include <random>
#include <sstream>

namespace soosh {

SooshSession::SooshSession()
    : gameStage_(GameStage::Waiting), numberOfRoundsCompleted_(0) {}

auto SooshSession::AddPlayer(const std::string &playerName) -> bool {
  if (gameStage_ != GameStage::Waiting) {
    return false;
  }
  for (const auto &player : players_) {
    if (player->GetName() == playerName) {
      return false;
    }
  }
  if (players_.size() >= playerLimit_) {
    return false;
  }
  players_.emplace_back(std::make_unique<Player>(playerName));
  return true;
}

auto SooshSession::RemovePlayer(const std::string &playerName) -> bool {
  auto it =
      std::ranges::find_if(players_, [&](const std::unique_ptr<Player> &p) {
        return p->GetName() == playerName;
      });
  if (it == players_.end()) {
    return false;
  }

  players_.erase(it);
  return true;
}

std::optional<std::string> SooshSession::Start() {
  if (players_.size() < 2) {
    return "At least two players required.";
  }
  switch (gameStage_) {
  case GameStage::Waiting:
    startRound();
    break;
  case GameStage::Playing:
    return "The game is already in-progress.";
  case GameStage::Finished:
    resetGame();
    startRound();
    break;
  }
  return {};
}

auto SooshSession::PlayCard(const std::string &playerName, int cardIndex1,
                            std::optional<int> cardIndex2) -> bool {
  if (gameStage_ != GameStage::Playing) {
    return false;
  }

  auto it =
      std::ranges::find_if(players_, [&](const std::unique_ptr<Player> &p_ptr) {
        return p_ptr->GetName() == playerName;
      });
  if (it == players_.end()) {
    return false;
  }
  Player &player = **it;
  if (player.HasFinishedTurn()) {
    return false;
  }

  if (cardIndex1 < 0 ||
      static_cast<size_t>(cardIndex1) >= player.GetHand().size()) {
    return false;
  }
  if (cardIndex2.has_value() &&
      (static_cast<size_t>(cardIndex2.value()) >= player.GetHand().size() ||
       cardIndex1 == cardIndex2.value() || !hasChopsticksInPlay(player))) {
    return false;
  }

  player.GetInPlay().push_back(player.GetHand()[cardIndex1]);
  player.GetHand().erase(player.GetHand().begin() + cardIndex1);

  if (cardIndex2.has_value()) {
    int index2 = cardIndex2.value();

    // Adjust index if it came after the first erased card
    if (index2 > cardIndex1) {
      --index2;
    }

    player.GetInPlay().push_back(player.GetHand()[index2]);
    player.GetHand().erase(player.GetHand().begin() + index2);

    auto &inPlay = player.GetInPlay();
    auto chopsticksIt = std::ranges::find_if(inPlay, [](const Card &card) {
      return card.GetType() == CardType::Chopsticks;
    });
    if (chopsticksIt != inPlay.end()) {
      player.GetHand().push_back(*chopsticksIt);
      inPlay.erase(chopsticksIt);
    }
  }

  player.SetFinishedTurn(true);

  if (std::all_of(players_.begin(), players_.end(),
                  [](const std::unique_ptr<Player> &p_ptr) {
                    return p_ptr->HasFinishedTurn();
                  })) {
    onTurnEnd();
  }

  return true;
}

GameStage SooshSession::GetGameStage() const { return gameStage_; }

const std::vector<std::unique_ptr<Player>> &SooshSession::GetPlayers() const {
  return players_;
}

void SooshSession::resetGame() {
  for (auto &player : players_) {
    player->GetInPlay().clear();
    player->GetHand().clear();
    player->SetPoints(0);
  }
  gameStage_ = GameStage::Waiting;
  numberOfRoundsCompleted_ = 0;
  resetDeck();
}

void SooshSession::startRound() {
  if (numberOfRoundsCompleted_ == 0) {
    resetDeck();
  }

  gameStage_ = GameStage::Playing;
  auto cardsPerPlayer = 10 - players_.size();
  for (auto &player : players_) {
    clearHandExceptPuddings(*player);
    for (int i = 0; i < cardsPerPlayer; ++i) {
      if (!deck_.empty()) {
        player->GetHand().push_back(deck_.top());
        deck_.pop();
      }
    }
  }
}

void SooshSession::onTurnEnd() {
  for (auto &player : players_) {
    player->SetFinishedTurn(false);
  }
  bool roundOver = std::any_of(players_.begin(), players_.end(),
                               [](const std::unique_ptr<Player> &player) {
                                 return player->GetHand().empty();
                               });
  if (roundOver) {
    ++numberOfRoundsCompleted_;
    if (numberOfRoundsCompleted_ == maxNumberOfRounds_) {
      gameStage_ = GameStage::Finished;
      scoring::CalculateRoundPoints(players_, true);
    } else {
      gameStage_ = GameStage::Waiting;
      scoring::CalculateRoundPoints(players_, false);
    }
  } else {
    rotateHands();
  }
}

void SooshSession::rotateHands() {
  if (players_.size() < 2) {
    return;
  }
  auto lastHand = players_.back()->GetHand();
  for (auto i = players_.size() - 1; i > 0; --i) {
    players_[i]->GetHand() = players_[i - 1]->GetHand();
  }
  players_[0]->GetHand() = lastHand;
}

void SooshSession::resetDeck() {
  deck_ = {};
  for (int i = 0; i < 14; ++i) {
    deck_.emplace(CardType::Tempura);
    deck_.emplace(CardType::Sashimi);
    deck_.emplace(CardType::Dumpling);
  }
  for (int i = 0; i < 12; ++i) {
    deck_.emplace(CardType::MakiRoll2);
  }
  for (int i = 0; i < 8; ++i) {
    deck_.emplace(CardType::MakiRoll3);
  }
  for (int i = 0; i < 6; ++i) {
    deck_.emplace(CardType::MakiRoll1);
  }
  for (int i = 0; i < 10; ++i) {
    deck_.emplace(CardType::SalmonNigiri);
  }
  for (int i = 0; i < 5; ++i) {
    deck_.emplace(CardType::SquidNigiri);
  }
  for (int i = 0; i < 5; ++i) {
    deck_.emplace(CardType::EggNigiri);
  }
  for (int i = 0; i < 10; ++i) {
    deck_.emplace(CardType::Pudding);
  }
  for (int i = 0; i < 6; ++i) {
    deck_.emplace(CardType::Wasabi);
  }
  for (int i = 0; i < 4; ++i) {
    deck_.emplace(CardType::Chopsticks);
  }

  // Shuffle the deck
  std::vector<Card> deckVec;
  while (!deck_.empty()) {
    deckVec.push_back(deck_.top());
    deck_.pop();
  }

  std::shuffle(deckVec.begin(), deckVec.end(), std::default_random_engine{});

  for (const auto &card : deckVec) {
    deck_.push(card);
  }
}

auto SooshSession::hasChopsticksInPlay(const Player &player) const -> bool {
  return std::ranges::any_of(player.GetInPlay(), [](const Card &card) {
    return card.GetType() == CardType::Chopsticks;
  });
}

void SooshSession::clearHandExceptPuddings(Player &player) {
  std::erase_if(player.GetInPlay(), [](const Card &card) {
    return card.GetType() != CardType::Pudding;
  });
}

auto SooshSession::SerializeGameState() const -> std::string {
  std::ostringstream oss;
  oss << "Round: " << (numberOfRoundsCompleted_ + 1) << "\n";
  for (const auto &player : players_) {
    oss << player->GetName() << " - " << player->GetPoints() << " points\n";
    for (const auto &card : player->GetInPlay()) {
      oss << "  " << card.ToString() << "\n";
    }
  }
  return oss.str();
}

auto SooshSession::SerializeHand(const std::string &playerName) const
    -> std::string {
  // todo make finding player& a private method
  auto it =
      std::ranges::find_if(players_, [&](const std::unique_ptr<Player> &p_ptr) {
        return p_ptr->GetName() == playerName;
      });
  if (it == players_.end()) {
    return "INVALID PLAYER\n"; // todo better error handling lol
  }
  Player &player = **it;

  std::ostringstream oss;

  if (gameStage_ != GameStage::Playing) {
    oss << "Waiting to start game...\n";

  } else if (!player.HasFinishedTurn()) {
    oss << "Your hand:\n";
    int index = 0;
    for (const auto &card : player.GetHand()) {
      oss << index << ". " << card.ToString() << "\n";
      ++index;
    }
  } else {
    oss << "Waiting for other players...\n";
  }

  return oss.str();
}

} // namespace soosh
