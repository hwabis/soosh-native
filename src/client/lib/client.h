#pragma once

#include <string>

namespace soosh
{

class Client
{
  public:
    void Start(const std::string &serverAddress);

  private:
    void print(const std::string &message) const;
};

} // namespace soosh
