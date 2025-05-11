#include "game_logic/soosh_session.h"
#include <gtest/gtest.h>

using namespace soosh;

TEST(PlayLogicTest, CannotPlayTwoTimes) {
  SooshSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  session.Start();
  EXPECT_TRUE(session.PlayCard("Alice", 0, {}));
  EXPECT_FALSE(session.PlayCard("Alice", 0, {}));
}

TEST(PlayLogicTest, RequireChopsticks) {
  SooshSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  session.Start();
  Player &alice = *session.GetPlayers()[0];

  alice.GetHand().clear();
  alice.GetInPlay().clear();
  alice.GetHand().emplace_back(CardType::Tempura);
  alice.GetHand().emplace_back(CardType::Dumpling);

  EXPECT_FALSE(session.PlayCard("Alice", 0, 1));

  alice.SetFinishedTurn(false);
  alice.GetHand().clear();
  alice.GetInPlay().clear();
  alice.GetInPlay().emplace_back(CardType::Chopsticks);
  alice.GetHand().emplace_back(CardType::Tempura);
  alice.GetHand().emplace_back(CardType::Dumpling);

  EXPECT_TRUE(session.PlayCard("Alice", 0, 1));
}

// todo there's more logic to validate lol
