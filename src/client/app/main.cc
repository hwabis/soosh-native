#include "client.h"
#include <CLI/CLI.hpp>

int main(int argc, char **argv)
{
    CLI::App app{"Client for Soosh Native"};
    argv = app.ensure_utf8(argv);

    std::string serverAddress;
    app.add_option("--connect", serverAddress, "Connect to the specified server address")
        ->required();

    CLI11_PARSE(app, argc, argv);

    soosh::Client client;
    client.Start(serverAddress);
}
