#pragma once

#include "game_logic/models/player.h"
#include <memory>

namespace soosh::scoring {

void CalculateRoundPoints(const std::vector<std::unique_ptr<Player>> &players,
                          bool includePudding);

} // namespace soosh::scoring
