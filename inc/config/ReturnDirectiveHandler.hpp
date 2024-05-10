#ifndef RETURN_DIRECTIVE_HANDLER_HPP
#define RETURN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ReturnDirectiveHandler : public ADirectiveHandler {
 public:
  ReturnDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
