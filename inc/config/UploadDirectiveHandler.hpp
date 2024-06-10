#ifndef UPLOAD_DIRECTIVE_HANDLER_HPP
#define UPLOAD_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class UploadDirectiveHandler : public ADirectiveHandler {
 public:
  UploadDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
};

#endif
