#include "game_logic/soosh_session.h"
#include <gtest/gtest.h>
#include <optional>

using namespace soosh;

TEST(JoinLeaveTest, AddPlayerSuccess) {
  SooshSession session;
  EXPECT_TRUE(session.AddPlayer("Alice"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
  EXPECT_EQ(session.GetPlayers()[0]->GetName(), "Alice");
}

TEST(JoinLeaveTest, AddPlayerDuplicateFails) {
  SooshSession session;
  session.AddPlayer("Alice");
  EXPECT_FALSE(session.AddPlayer("Alice"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
}

TEST(JoinLeaveTest, AddPlayerAfterGameStartedFails) {
  SooshSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  EXPECT_TRUE(session.StartGame().has_value() == false);
  EXPECT_FALSE(session.AddPlayer("Charlie"));
}

TEST(JoinLeaveTest, StartGameFailsWithLessThanTwoPlayers) {
  SooshSession session;
  session.AddPlayer("Alice");
  std::optional<std::string> error = session.StartGame();
  EXPECT_TRUE(error.has_value());
  EXPECT_EQ(error.value(), "At least two players required.");
}

TEST(JoinLeaveTest, StartGameSucceedsWithTwoPlayers) {
  SooshSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");
  EXPECT_TRUE(session.StartGame().has_value() == false);
  EXPECT_EQ(session.GetGameStage(), GameStage::Playing);
}

TEST(JoinLeaveTest, PlayerIsRemovedSuccessfully) {
  SooshSession session;
  session.AddPlayer("Alice");
  session.AddPlayer("Bob");

  bool removed = session.RemovePlayer("Alice");
  EXPECT_TRUE(removed);

  ASSERT_EQ(session.GetPlayers().size(), 1);
  if (!session.GetPlayers().empty()) {
    EXPECT_EQ(session.GetPlayers()[0]->GetName(), "Bob");
  }
}

TEST(JoinLeaveTest, RemoveNonexistentPlayerFails) {
  SooshSession session;
  session.AddPlayer("Alice");
  EXPECT_FALSE(session.RemovePlayer("Bob"));
  EXPECT_EQ(session.GetPlayers().size(), 1);
}
