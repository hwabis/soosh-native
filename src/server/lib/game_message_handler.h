#pragma once

#include "soosh.pb.h"
#include <memory>
#include <string>

namespace soosh {

class ServerSession;

class GameMessageHandler {
public:
  void OnMessageReceived(const soosh::ClientMessage &message,
                         ServerSession &session);
};

} // namespace soosh
