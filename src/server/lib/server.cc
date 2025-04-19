#include "server.h"
#include <boost/asio.hpp>
#include <iostream>

namespace soosh
{

Server::Server(unsigned short port)
    : ioContext_(), acceptor_(ioContext_, ip::tcp::endpoint(ip::tcp::v4(), port))
{
    startAccept();
}

void Server::Run()
{
    print("Server started!");
    ioContext_.run();
}

void Server::startAccept()
{
    auto socket = std::make_shared<ip::tcp::socket>(ioContext_);
    acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &ec) {
        if (!ec)
        {
            print("Client connected!");
            readMessage(socket);
            sendMessage(socket, "Hello from encapsulated server!\n");
        }
        startAccept();
    });
}

void Server::readMessage(std::shared_ptr<ip::tcp::socket> socket)
{
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(
        *socket, *buffer, '\n',
        [this, socket, buffer](const boost::system::error_code &ec, std::size_t bytesTransferred) {
            if (!ec)
            {
                std::istream input(buffer.get());
                std::string message;
                std::getline(input, message);

                print(message);

                readMessage(socket);
            }
            else
            {
                if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
                {
                    print("Client disconnected.");
                }
                else
                {
                    print("Error receiving message: " + ec.message());
                }
            }
        });
}

void Server::sendMessage(std::shared_ptr<ip::tcp::socket> socket, const std::string &message)
{
    auto buffer = std::make_shared<std::string>(message);
    boost::asio::async_write(
        *socket, boost::asio::buffer(*buffer),
        [this, socket, buffer](const boost::system::error_code &ec, std::size_t bytesTransferred) {
            if (!ec)
            {
                print("Message sent to client.");

                auto timer = std::make_shared<boost::asio::steady_timer>(
                    ioContext_, boost::asio::chrono::seconds(5));
                timer->async_wait([this, socket, timer](const boost::system::error_code &ec) {
                    if (!ec)
                    {
                        sendMessage(socket, "Another message from the server after a delay.\n");
                    }
                });
            }
            else
            {
                if (ec == boost::asio::error::connection_reset || ec == boost::asio::error::eof)
                {
                    print("Client disconnected. No further messages will be sent to this client.");
                }
                else
                {
                    print("Error sending message: " + ec.message());
                }
            }
        });
}

void Server::print(const std::string &message) const
{
    std::cout << message << std::endl;
}

} // namespace soosh
