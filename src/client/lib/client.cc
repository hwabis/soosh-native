#include "client.h"
#include <iostream>

namespace soosh
{

Client::Client(const std::string &serverAddress, unsigned short port)
    : ioContext_(), socket_(ioContext_), serverEndpoint_(ip::make_address(serverAddress), port)
{
}

void Client::Start()
{
    try
    {
        socket_.connect(serverEndpoint_);
        print("Connected to server at " + serverEndpoint_.address().to_string() + ":" +
              std::to_string(serverEndpoint_.port()));
        listenForMessages();
        ioContext_.run();
    }
    catch (const boost::system::system_error &e)
    {
        print("Connection failed: " + std::string(e.what()));
    }
}

void Client::listenForMessages()
{
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(
        socket_, *buffer, '\n',
        [this, buffer](const boost::system::error_code &ec, std::size_t bytesTransferred) {
            if (!ec)
            {
                std::istream input(buffer.get());
                std::string message;
                std::getline(input, message);
                std::cout << "Server says: " << message << std::endl;

                listenForMessages();
            }
            else
            {
                std::cerr << "Error while receiving message: " << ec.message() << std::endl;
            }
        });
}

void Client::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
