#include <mutex>
#include <string_view>

namespace soosh {

class Logger {
public:
  enum class Level { Info, Warning, Error };

  static void Log(std::string_view msg, Level level = Level::Info);

private:
  static std::mutex logMutex_;
};

} // namespace soosh
