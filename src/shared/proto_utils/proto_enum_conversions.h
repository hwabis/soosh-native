#pragma once

#include "soosh.pb.h"
#include <string>

namespace soosh::utils {

std::string ActionTypeToString(ActionType action) {
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
