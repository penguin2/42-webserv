#ifndef INDEX_DIRECTIVE_HANDLER_HPP
#define INDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class IndexDirectiveHandler : public ADirectiveHandler {
 public:
  IndexDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
