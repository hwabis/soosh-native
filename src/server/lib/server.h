#pragma once

#include <string>

namespace soosh
{

class Server
{
  public:
    void Run(int port);

  private:
    void print(const std::string &message) const;
};

} // namespace soosh
