#pragma once

#include "soosh.pb.h"

namespace soosh::utils {

auto ActionTypeToString(ActionType action) -> std::string;
auto StatusTypeToString(StatusType status) -> std::string;

} // namespace soosh::utils
