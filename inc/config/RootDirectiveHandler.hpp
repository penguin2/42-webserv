#ifndef ROOT_DIRECTIVE_HANDLER_HPP
#define ROOT_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class RootDirectiveHandler : public ADirectiveHandler {
 public:
  RootDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
