#include "server.h"
#include <boost/asio.hpp>
#include <iostream>

namespace ip = boost::asio::ip;

namespace soosh
{

void Server::Run(int port)
{
    ip::tcp::endpoint endpoint(ip::address_v4::any(), static_cast<unsigned short>(port));
}

void Server::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
