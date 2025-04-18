#include "server.h"
#include <CLI/CLI.hpp>

int main(int argc, char **argv)
{
    CLI::App app{"Server for Soosh Native"};
    argv = app.ensure_utf8(argv);

    int port;
    app.add_option("--port", port, "Port to serve on")->required();

    CLI11_PARSE(app, argc, argv);

    soosh::Server server;
    server.Run(port);
}
