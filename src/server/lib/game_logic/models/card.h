#pragma once

#include <string>

namespace soosh {

enum class CardType : std::uint8_t {
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
  [[nodiscard]] auto GetType() const -> CardType { return type_; }
  [[nodiscard]] auto ToString() const -> std::string; // todo lol

private:
  CardType type_;
};

} // namespace soosh
