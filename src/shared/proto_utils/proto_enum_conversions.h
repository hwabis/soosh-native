#pragma once

#include "soosh.pb.h"

namespace soosh {
namespace utils {

auto ActionTypeToString(ActionType action) -> std::string;
auto StatusTypeToString(StatusType status) -> std::string;

} // namespace utils
} // namespace soosh
