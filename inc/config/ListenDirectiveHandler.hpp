#ifndef LISTEN_DIRECTIVE_HANDLER_HPP
#define LISTEN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ListenDirectiveHandler : public ADirectiveHandler {
 public:
  ListenDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
  static bool parseAddressAndPort(const std::string& str, std::string& address,
                                  std::string& port);
};

#endif
