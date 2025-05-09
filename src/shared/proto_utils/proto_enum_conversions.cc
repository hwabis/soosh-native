#include "proto_enum_conversions.h"
#include <string>

namespace soosh {
namespace utils {

auto ActionTypeToString(ActionType action) -> std::string {
  switch (action) {
  case ActionType::Unknown:
    return "Unknown";
  case ActionType::Join:
    return "Join";
  case ActionType::Start:
    return "Start";
  case ActionType::Play:
    return "Play";
  default:
    return "Invalid action";
  }
}

auto StatusTypeToString(StatusType status) -> std::string {
  switch (status) {
  case StatusType::Error:
    return "Error";
  case StatusType::Update:
    return "Update";
  default:
    return "Invalid status";
  }
}

} // namespace utils
} // namespace soosh
