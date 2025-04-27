#pragma once

#include <string>

namespace soosh {

enum class CardType {
  Tempura,
  Sashimi,
  Dumpling,
  MakiRoll1,
  MakiRoll2,
  MakiRoll3,
  SalmonNigiri,
  SquidNigiri,
  EggNigiri,
  Wasabi,
  Pudding,
  Chopsticks
};

class Card {
public:
  explicit Card(CardType type) : type_(type) {}
  CardType GetType() const { return type_; }
  std::string ToString() const;

private:
  CardType type_;
};

} // namespace soosh
