#include "client_ui.h"
#include <iostream>

namespace soosh {

auto ClientUi::PromptInput(std::string_view prompt) -> std::string {
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << ANSI_COLOR_GREEN << prompt << ANSI_COLOR_RESET << '\n';

  std::string input;
  std::getline(std::cin, input);
  return input;
}

void ClientUi::PrintStatus(std::string_view msg) {
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << ANSI_COLOR_YELLOW << msg << ANSI_COLOR_RESET << "\n";
}

void ClientUi::PrintError(std::string_view msg) {
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << ANSI_COLOR_RED << msg << ANSI_COLOR_RESET << "\n";
}

} // namespace soosh
