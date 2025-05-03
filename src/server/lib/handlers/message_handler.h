#pragma once

#include "soosh.pb.h"

namespace soosh {

class ClientSession;

class IMessageHandler {
public:
  virtual void OnMessageReceived(const soosh::ClientMessage &message,
                                 ClientSession &session) = 0;
  virtual ~IMessageHandler() = default;
};

} // namespace soosh
