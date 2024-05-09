#ifndef SERVER_NAME_DIRECTIVE_HANDLER_HPP
#define SERVER_NAME_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ServerNameDirectiveHandler : public ADirectiveHandler {
 public:
  ServerNameDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
