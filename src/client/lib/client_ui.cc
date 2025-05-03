#include "client_ui.h"
#include <iostream>

namespace soosh {

const std::string ClientUi::ANSI_COLOR_RED = "\033[31m";
const std::string ClientUi::ANSI_COLOR_GREEN = "\033[32m";
const std::string ClientUi::ANSI_COLOR_YELLOW = "\033[33m";
const std::string ClientUi::ANSI_COLOR_RESET = "\033[0m";

std::string ClientUi::PromptInput(std::string_view prompt) {
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << ANSI_COLOR_GREEN << prompt << ANSI_COLOR_RESET << std::endl;

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
