#ifndef AUTOINDEX_DIRECTIVE_HANDLER_HPP
#define AUTOINDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class AutoIndexDirectiveHandler : public ADirectiveHandler {
 public:
  AutoIndexDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig();
};

#endif
