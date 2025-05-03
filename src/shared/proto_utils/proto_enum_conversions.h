#pragma once

#include "soosh.pb.h"
#include <string>

namespace soosh::utils {

std::string ActionTypeToString(ActionType action) {
  switch (action) {
  case ActionType::UNKNOWN:
    return "Unknown";
  case ActionType::JOIN:
    return "Join";
  case ActionType::START:
    return "Start";
  case ActionType::PLAY:
    return "Play";
  default:
    return "Invalid action";
  }
}

std::string StatusTypeToString(StatusType status) {
  switch (status) {
  case StatusType::Error:
    return "Error";
  case StatusType::Update:
    return "Update";
  default:
    return "Invalid status";
  }
}

} // namespace soosh::utils
