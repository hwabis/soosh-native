#include "networking/server.h"
#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
  CLI::App app;
  argv = app.ensure_utf8(argv);

  unsigned short port = 0;
  app.add_option("-p,--port", port, "Port to serve on")->required();

  CLI11_PARSE(app, argc, argv);

  auto server = std::make_shared<soosh::Server>(port);
  server->Start();
}
