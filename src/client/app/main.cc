#include "client.h"
#include <CLI/CLI.hpp>
#include <string>

auto main(int argc, char **argv) -> int {
  CLI::App app;
  argv = app.ensure_utf8(argv);

  std::string serverAddress;
  app.add_option("-c,--connect", serverAddress, "Address to connect to")
      ->required();
  unsigned short port = 0;
  app.add_option("-p,--port", port, "Port to connect to")->required();

  CLI11_PARSE(app, argc, argv);

  soosh::Client client(serverAddress, port);
  client.Start();
}
