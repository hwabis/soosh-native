#pragma once

#include <mutex>
#include <string>
#include <string_view>

namespace soosh {

class ClientUi {
public:
  std::string PromptInput(std::string_view prompt);
  void PrintStatus(std::string_view msg);
  void PrintError(std::string_view msg);

private:
  std::mutex mutex_;

  static const std::string ANSI_COLOR_RED;
  static const std::string ANSI_COLOR_GREEN;
  static const std::string ANSI_COLOR_YELLOW;
  static const std::string ANSI_COLOR_RESET;
};

} // namespace soosh
