#include "logger.h"
#include <iostream>

namespace soosh {

std::mutex Logger::logMutex_;

void Logger::Log(std::string_view msg, Level level) {
  std::string levelStr;
  switch (level) {
  case Level::Info:
    levelStr = "[INFO] ";
    break;
  case Level::Warning:
    levelStr = "[WARNING] ";
    break;
  case Level::Error:
    levelStr = "[ERROR] ";
    break;
  }

  std::lock_guard<std::mutex> lock(logMutex_);
  std::cout << levelStr << msg << std::endl;
}

} // namespace soosh
