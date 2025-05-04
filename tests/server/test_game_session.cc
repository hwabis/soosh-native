#include "game_logic/game_session.h"
#include <gtest/gtest.h>

using namespace soosh;

TEST(GameSessionTest, AddPlayerSuccess) {
  GameSession session;
  EXPECT_TRUE(session.AddPlayer("Alice"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
  EXPECT_EQ(session.GetPlayers()[0]->GetName(), "Alice");
}

TEST(GameSessionTest, AddPlayerDuplicateFails) {
  GameSession session;
  session.AddPlayer("Alice");
  EXPECT_FALSE(session.AddPlayer("Alice"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
}

TEST(GameSessionTest, AddPlayerAfterGameStartedFails) {
  GameSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  std::string error;
  ASSERT_TRUE(session.StartGame(error));
  EXPECT_FALSE(session.AddPlayer("Charlie"));
}

TEST(GameSessionTest, StartGameFailsWithLessThanTwoPlayers) {
  GameSession session;
  session.AddPlayer("Alice");
  std::string error;
  EXPECT_FALSE(session.StartGame(error));
  EXPECT_EQ(error, "At least two players required.");
}

TEST(GameSessionTest, StartGameSucceedsWithTwoPlayers) {
  GameSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  std::string error;
  EXPECT_TRUE(session.StartGame(error));
  EXPECT_EQ(session.GetGameStage(), GameStage::Playing);
}

TEST(GameSessionTest, PlayerIsRemovedSuccessfully) {
  GameSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");

  bool removed = session.RemovePlayer("Alice");
  EXPECT_TRUE(removed);

  ASSERT_EQ(session.GetPlayers().size(), 1);
  if (!session.GetPlayers().empty()) {
    EXPECT_EQ(session.GetPlayers()[0]->GetName(), "Bob");
  }
}

TEST(GameSessionTest, RemoveNonexistentPlayerFails) {
  GameSession session;
  session.AddPlayer("Alice");
  EXPECT_FALSE(session.RemovePlayer("Bob"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
}
