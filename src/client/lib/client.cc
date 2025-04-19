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
        std::jthread inputThread(&Client::handleInput, this);
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
                print("Server says: " + message);
                listenForMessages();
            }
            else
            {
                if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
                {
                    print("Server disconnected.");
                    socket_.close();
                }
                else
                {
                    print("Error while receiving message: " + ec.message());
                }
            }
        });
}

void Client::handleInput()
{
    try
    {
        while (true)
        {
            std::string userInput;
            std::cout << "> ";
            std::getline(std::cin, userInput);

            if (userInput.empty())
            {
                continue;
            }

            if (userInput == "quit")
            {
                print("Exiting client...");
                socket_.close();
                break;
            }

            // Send the message to the server
            boost::asio::write(socket_, boost::asio::buffer(userInput + "\n"));
        }
    }
    catch (const boost::system::system_error &e)
    {
        print("Error sending message: " + std::string(e.what()));
    }
}

void Client::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
