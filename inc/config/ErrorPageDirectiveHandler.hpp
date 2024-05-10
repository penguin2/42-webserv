#ifndef ERROR_PAGE_DIRECTIVE_HANDLER_HPP
#define ERROR_PAGE_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ErrorPageDirectiveHandler : public ADirectiveHandler {
 public:
  ErrorPageDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
