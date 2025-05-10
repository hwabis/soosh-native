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
  [[nodiscard]] auto ToString() const -> std::string {
    switch (type_) {
    case CardType::Tempura:
      return "Tempura";
    case CardType::Sashimi:
      return "Sashimi";
    case CardType::Dumpling:
      return "Dumpling";
    case CardType::MakiRoll1:
      return "Maki Roll (1)";
    case CardType::MakiRoll2:
      return "Maki Roll (2)";
    case CardType::MakiRoll3:
      return "Maki Roll (3)";
    case CardType::SalmonNigiri:
      return "Salmon Nigiri";
    case CardType::SquidNigiri:
      return "Squid Nigiri";
    case CardType::EggNigiri:
      return "Egg Nigiri";
    case CardType::Wasabi:
      return "Wasabi";
    case CardType::Pudding:
      return "Pudding";
    case CardType::Chopsticks:
      return "Chopsticks";
    default:
      return "??";
    }
  }

private:
  CardType type_;
};

} // namespace soosh
