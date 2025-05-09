#pragma once

#include "soosh.pb.h"

namespace soosh {

class ClientSession;

class IMessageHandler {
public:
  IMessageHandler() = default;
  IMessageHandler(const IMessageHandler &) = delete;
  auto operator=(const IMessageHandler &) -> IMessageHandler & = delete;
  IMessageHandler(IMessageHandler &&) = delete;
  auto operator=(IMessageHandler &&) -> IMessageHandler & = delete;

  virtual void OnMessageReceived(const soosh::ClientMessage &message,
                                 std::shared_ptr<ClientSession> session) = 0;
  virtual void OnClientDisconnected(std::shared_ptr<ClientSession> session) = 0;
  virtual ~IMessageHandler() = default;
};

} // namespace soosh
