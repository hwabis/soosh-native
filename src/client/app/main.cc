#include "client.h"
#include <CLI/CLI.hpp>

int main(int argc, char **argv)
{
    CLI::App app{"Client for Soosh Native"};
    argv = app.ensure_utf8(argv);

    std::string serverAddress;
    app.add_option("--connect", serverAddress, "Server address to connect to")->required();

    CLI11_PARSE(app, argc, argv);

    soosh::Client client;
    client.Run(serverAddress);
}
