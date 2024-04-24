#ifndef TRY_FILES_DIRECTIVE_HANDLER_HPP
#define TRY_FILES_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class TryFilesDirectiveHandler : public ADirectiveHandler {
 public:
  TryFilesDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
