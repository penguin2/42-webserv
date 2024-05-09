#ifndef LISTEN_DIRECTIVE_HANDLER_HPP
#define LISTEN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ListenDirectiveHandler : public ADirectiveHandler {
 public:
  ListenDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
  void parseAddressAndPort(const std::string& input, std::string& address,
                           int& port);
};

#endif
