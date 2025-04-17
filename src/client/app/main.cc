#include "henlo.h"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "henlo";
    noooo();

    CLI::App app{"Client for Soosh Native"};
    argv = app.ensure_utf8(argv);

    std::string serverAddress;
    app.add_option("--connect", serverAddress, "Connect to the specified server address")
        ->required();

    CLI11_PARSE(app, argc, argv);
}
