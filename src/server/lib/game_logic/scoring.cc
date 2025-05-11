#include "game_logic/scoring.h"
#include <algorithm>
#include <array>
#include <limits>
#include <unordered_map>

namespace soosh::scoring {

namespace {

void addMakiRollPoints(const std::unordered_map<Player *, int> &counts,
                       std::unordered_map<Player *, int> &points) {
  int max = 0;
  for (const auto &[_, c] : counts)
    max = std::max(max, c);

  std::vector<Player *> first, second;
  for (const auto &[p, c] : counts)
    if (c == max && max > 0)
      first.push_back(p);

  int secondMax = 0;
  for (const auto &[_, c] : counts)
    if (c < max)
      secondMax = std::max(secondMax, c);

  for (const auto &[p, c] : counts)
    if (c == secondMax && secondMax > 0)
      second.push_back(p);

  if (!first.empty()) {
    int pts = 6 / static_cast<int>(first.size());
    for (auto *p : first)
      points[p] += pts;
  }
  if (!second.empty()) {
    int pts = 3 / static_cast<int>(second.size());
    for (auto *p : second)
      points[p] += pts;
  }
}

void addPuddingPoints(const std::unordered_map<Player *, int> &counts,
                      std::unordered_map<Player *, int> &points) {
  int max = 0, min = std::numeric_limits<int>::max();
  for (const auto &[_, c] : counts) {
    max = std::max(max, c);
    min = std::min(min, c);
  }

  if (max == min)
    return;

  std::vector<Player *> most, least;
  for (const auto &[p, c] : counts) {
    if (c == max)
      most.push_back(p);
    if (c == min)
      least.push_back(p);
  }

  if (!most.empty()) {
    int pts = 6 / static_cast<int>(most.size());
    for (auto *p : most)
      points[p] += pts;
  }

  if (counts.size() > 2 && !least.empty()) {
    int pts = -6 / static_cast<int>(least.size());
    for (auto *p : least)
      points[p] += pts;
  }
}

} // namespace

void CalculateRoundPoints(const std::vector<std::unique_ptr<Player>> &players,
                          bool includePudding) {
  std::unordered_map<Player *, int> points;
  std::unordered_map<Player *, int> makiCounts;
  std::unordered_map<Player *, int> puddingCounts;

  for (const auto &ptr : players) {
    Player *p = ptr.get();
    points[p] = 0;
    makiCounts[p] = 0;
    puddingCounts[p] = 0;

    int tempura = 0, sashimi = 0, dumpling = 0;
    bool wasabi = false;

    for (const auto &card : p->GetInPlay()) {
      switch (card.GetType()) {
      case CardType::Tempura:
        tempura++;
        break;
      case CardType::Sashimi:
        sashimi++;
        break;
      case CardType::Dumpling:
        dumpling++;
        break;
      case CardType::Wasabi:
        wasabi = true;
        break;
      case CardType::SquidNigiri:
        points[p] += wasabi ? 9 : 3;
        wasabi = false;
        break;
      case CardType::SalmonNigiri:
        points[p] += wasabi ? 6 : 2;
        wasabi = false;
        break;
      case CardType::EggNigiri:
        points[p] += wasabi ? 3 : 1;
        wasabi = false;
        break;
      case CardType::MakiRoll3:
        makiCounts[p] += 3;
        break;
      case CardType::MakiRoll2:
        makiCounts[p] += 2;
        break;
      case CardType::MakiRoll1:
        makiCounts[p] += 1;
        break;
      case CardType::Pudding:
        puddingCounts[p]++;
        break;
      default:
        break;
      }
    }

    points[p] += (tempura / 2) * 5;
    points[p] += (sashimi / 3) * 10;
    points[p] +=
        std::min(15, std::array{0, 1, 3, 6, 10, 15}[std::min(dumpling, 5)]);
  }

  addMakiRollPoints(makiCounts, points);
  if (includePudding) {
    addPuddingPoints(puddingCounts, points);
  }

  for (auto &[p, score] : points) {
    p->SetPoints(p->GetPoints() + score);
  }
}

} // namespace soosh::scoring
