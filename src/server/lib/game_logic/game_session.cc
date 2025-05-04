#include "game_logic/game_session.h"
#include <algorithm>
#include <random>
#include <sstream>

namespace soosh {

GameSession::GameSession()
    : gameStage_(GameStage::Waiting), numberOfRoundsCompleted_(0) {}

bool GameSession::AddPlayer(const std::string &playerName) {
  if (gameStage_ != GameStage::Waiting)
    return false;
  for (const auto &player : players_) {
    if (player->GetName() == playerName)
      return false;
  }
  players_.emplace_back(std::make_unique<Player>(playerName));
  return true;
}

bool GameSession::RemovePlayer(const std::string &playerName) {
  auto it = std::find_if(players_.begin(), players_.end(),
                         [&](const std::unique_ptr<Player> &p) {
                           return p->GetName() == playerName;
                         });
  if (it == players_.end())
    return false;

  players_.erase(it);
  return true;
}

bool GameSession::StartGame(std::string &error) {
  if (players_.size() < 2) {
    error = "At least two players required.";
    return false;
  }
  resetGame();
  gameStage_ = GameStage::Playing;
  distributeCards();
  return true;
}

bool GameSession::PlayCard(const std::string &playerName, int cardIndex1,
                           int cardIndex2, std::string &error) {
  auto it = std::find_if(players_.begin(), players_.end(),
                         [&](const std::unique_ptr<Player> &p_ptr) {
                           return p_ptr->GetName() == playerName;
                         });
  if (it == players_.end()) {
    error = "Player not found.";
    return false;
  }
  Player &player = **it;

  if (cardIndex1 < 0 || cardIndex1 >= player.GetHand().size()) {
    error = "Invalid card index.";
    return false;
  }
  if (cardIndex2 >= 0 &&
      (cardIndex2 >= player.GetHand().size() || cardIndex1 == cardIndex2)) {
    error = "Invalid second card index.";
    return false;
  }

  player.GetEnqueuedCardsToPlay().push_back(player.GetHand()[cardIndex1]);
  if (cardIndex2 >= 0) {
    player.GetEnqueuedCardsToPlay().push_back(player.GetHand()[cardIndex2]);
  }
  player.SetFinishedTurn(true);

  if (std::all_of(players_.begin(), players_.end(),
                  [](const std::unique_ptr<Player> &p_ptr) {
                    return p_ptr->HasFinishedTurn();
                  })) {
    advanceRound();
  }

  return true;
}

GameStage GameSession::GetGameStage() const { return gameStage_; }

const std::vector<std::unique_ptr<Player>> &GameSession::GetPlayers() const {
  return players_;
}

void GameSession::resetGame() {
  numberOfRoundsCompleted_ = 0;
  initDeck();
}

void GameSession::resetRound() {
  for (auto &player_ptr : players_) {
    player_ptr->GetInPlay().clear();
    player_ptr->GetHand().clear();
  }
}

void GameSession::distributeCards() {
  auto cardsPerPlayer = 10 - players_.size();
  for (auto &player_ptr : players_) {
    for (int i = 0; i < cardsPerPlayer; ++i) {
      if (!deck_.empty()) {
        player_ptr->GetHand().push_back(deck_.top());
        deck_.pop();
      }
    }
  }
}

void GameSession::rotateHands() {
  if (players_.size() < 2)
    return;
  auto lastHand = players_.back()->GetHand();
  for (auto i = players_.size() - 1; i > 0; --i) {
    players_[i]->GetHand() = players_[i - 1]->GetHand();
  }
  players_[0]->GetHand() = lastHand;
}

bool GameSession::checkRoundEnd() {
  return std::any_of(players_.begin(), players_.end(),
                     [](const std::unique_ptr<Player> &p_ptr) {
                       return p_ptr->GetHand().empty();
                     });
}

bool GameSession::checkGameEnd() { return numberOfRoundsCompleted_ >= 3; }

void GameSession::advanceRound() {
  for (auto &player_ptr : players_) {
    player_ptr->SetFinishedTurn(false);
    player_ptr->GetInPlay().insert(player_ptr->GetInPlay().end(),
                                   player_ptr->GetEnqueuedCardsToPlay().begin(),
                                   player_ptr->GetEnqueuedCardsToPlay().end());
    player_ptr->GetEnqueuedCardsToPlay().clear();
  }

  if (checkRoundEnd()) {
    ++numberOfRoundsCompleted_;
    if (checkGameEnd()) {
      gameStage_ = GameStage::Finished;
    } else {
      rotateHands();
    }
  }
}

void GameSession::initDeck() {
  deck_ = {};
  for (int i = 0; i < 14; ++i) {
    deck_.push(Card(CardType::Tempura));
    deck_.push(Card(CardType::Sashimi));
    deck_.push(Card(CardType::Dumpling));
  }
  for (int i = 0; i < 12; ++i)
    deck_.push(Card(CardType::MakiRoll2));
  for (int i = 0; i < 8; ++i)
    deck_.push(Card(CardType::MakiRoll3));
  for (int i = 0; i < 6; ++i)
    deck_.push(Card(CardType::MakiRoll1));
  for (int i = 0; i < 10; ++i)
    deck_.push(Card(CardType::SalmonNigiri));
  for (int i = 0; i < 5; ++i)
    deck_.push(Card(CardType::SquidNigiri));
  for (int i = 0; i < 5; ++i)
    deck_.push(Card(CardType::EggNigiri));
  for (int i = 0; i < 10; ++i)
    deck_.push(Card(CardType::Pudding));
  for (int i = 0; i < 6; ++i)
    deck_.push(Card(CardType::Wasabi));
  for (int i = 0; i < 4; ++i)
    deck_.push(Card(CardType::Chopsticks));

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

std::string GameSession::SerializeGameState() const {
  std::ostringstream oss;
  oss << "Stage: " << static_cast<int>(gameStage_) << "\n";
  for (const auto &player_ptr : players_) {
    oss << player_ptr->GetName() << " - Hand: " << player_ptr->GetHand().size()
        << " cards, Points: " << player_ptr->GetPoints() << "\n";
  }
  return oss.str();
}
} // namespace soosh
