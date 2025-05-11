#include "game_logic/models/player.h"
#include "game_logic/scoring.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace soosh;

std::unique_ptr<Player> MakePlayer(std::string name) {
  return std::make_unique<Player>(std::move(name));
}

TEST(ScoringTest, TestPointsWithNoPlayerConditions) {
  std::vector<std::unique_ptr<Player>> players;

  auto p1 = MakePlayer("player1");
  auto p2 = MakePlayer("player2");
  auto p3 = MakePlayer("player3");

  p1->GetInPlay() = {
      Card(CardType::Tempura),  Card(CardType::Tempura), // 5
      Card(CardType::Tempura),  Card(CardType::Sashimi),
      Card(CardType::Sashimi),  Card(CardType::Sashimi), // 15
      Card(CardType::Sashimi),  Card(CardType::Dumpling),
      Card(CardType::Dumpling), Card(CardType::Dumpling),
      Card(CardType::Dumpling), Card(CardType::Dumpling) // 15 for dumpling
  };

  p2->GetInPlay() = {
      Card(CardType::Tempura), Card(CardType::Sashimi), Card(CardType::Sashimi),
      Card(CardType::Dumpling) // 1 point
  };

  p3->GetInPlay() = {Card(CardType::SquidNigiri), Card(CardType::SalmonNigiri),
                     Card(CardType::EggNigiri),   Card(CardType::Wasabi),
                     Card(CardType::Wasabi),      Card(CardType::EggNigiri),
                     Card(CardType::EggNigiri),   Card(CardType::Wasabi),
                     Card(CardType::SquidNigiri)};

  players.push_back(std::move(p1));
  players.push_back(std::move(p2));
  players.push_back(std::move(p3));

  scoring::CalculateRoundPoints(players, false);

  EXPECT_EQ(players[0]->GetPoints(), 30);
  EXPECT_EQ(players[1]->GetPoints(), 1);
  EXPECT_EQ(players[2]->GetPoints(), 19);
}

// todo maki roll and pudding
