#pragma once

#include <memory>
#include <string>

namespace soosh {

class Session;

class GameMessageHandler {
public:
  void OnMessageReceived(const std::string &message,
                         std::shared_ptr<Session> session);
};

} // namespace soosh
