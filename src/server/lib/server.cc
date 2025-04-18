#include "server.h"
#include <iostream>

namespace soosh
{

void Server::Run(int port)
{
}

void Server::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
