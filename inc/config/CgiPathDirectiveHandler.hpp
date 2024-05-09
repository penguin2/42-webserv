#ifndef CGI_PATH_DIRECTIVE_HANDLER_HPP
#define CGI_PATH_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class CgiPathDirectiveHandler : public ADirectiveHandler {
 public:
  CgiPathDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
