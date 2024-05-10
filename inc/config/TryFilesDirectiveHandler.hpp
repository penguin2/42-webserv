#ifndef TRY_FILES_DIRECTIVE_HANDLER_HPP
#define TRY_FILES_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class TryFilesDirectiveHandler : public ADirectiveHandler {
 public:
  TryFilesDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
