#pragma once

#include <mutex>
#include <string>
#include <string_view>

namespace soosh {

class ClientUi {
public:
  auto PromptInput(std::string_view prompt) -> std::string;
  void PrintStatus(std::string_view msg);
  void PrintError(std::string_view msg);

private:
  std::mutex mutex_;

  static constexpr std::string_view ANSI_COLOR_RED = "\033[31m";
  static constexpr std::string_view ANSI_COLOR_GREEN = "\033[32m";
  static constexpr std::string_view ANSI_COLOR_YELLOW = "\033[33m";
  static constexpr std::string_view ANSI_COLOR_RESET = "\033[0m";
};

} // namespace soosh
