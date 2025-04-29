#pragma once

#include "soosh.pb.h"

namespace soosh {

class ServerSession;

class IMessageHandler {
public:
  virtual void OnMessageReceived(const soosh::ClientMessage &message,
                                 ServerSession &session) = 0;
  virtual ~IMessageHandler() = default;
};

} // namespace soosh
