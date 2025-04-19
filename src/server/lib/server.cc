#include "server.h"
#pragma warning(push)
#pragma warning(disable : 6387 6258 6001 6255 6031 6313)
#include <boost/asio.hpp>
#pragma warning(pop)
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
