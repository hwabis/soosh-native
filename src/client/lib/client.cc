#include "client.h"
#include <iostream>

namespace soosh
{

void Client::Run(const std::string &serverAddress)
{
}

void Client::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
