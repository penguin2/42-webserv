#ifndef AUTOINDEX_DIRECTIVE_HANDLER_HPP
#define AUTOINDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class AutoIndexDirectiveHandler : public ADirectiveHandler {
 public:
  AutoIndexDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
