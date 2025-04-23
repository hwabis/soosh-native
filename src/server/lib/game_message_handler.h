#pragma once

#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class Session;

class GameMessageHandler {
public:
  void OnMessageReceived(const soosh::ClientMessage &message, Session &session);
};

} // namespace soosh
