#pragma once

#include <boost/asio.hpp>
#include <string>

namespace ip = boost::asio::ip;

namespace soosh
{

class Server
{
  public:
    explicit Server(unsigned short port);
    void Start();

  private:
    void startAccept();
    void readMessage(std::shared_ptr<ip::tcp::socket> socket);
    void sendMessage(std::shared_ptr<ip::tcp::socket> socket, const std::string &message);
    void print(const std::string &message) const;
    boost::asio::io_context ioContext_;
    ip::tcp::acceptor acceptor_;
};

} // namespace soosh
