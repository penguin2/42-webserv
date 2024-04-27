#ifndef LISTEN_DIRECTIVE_HANDLER_HPP
#define LISTEN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ListenDirectiveHandler : public ADirectiveHandler {
 public:
  ListenDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
  void parseAddressAndPort(const std::string& input, std::string& address,
                           int& port);
};

#endif
