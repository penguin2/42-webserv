#ifndef TRY_FILES_DIRECTIVE_HANDLER_HPP
#define TRY_FILES_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class TryFilesDirectiveHandler : public ADirectiveHandler {
 public:
  TryFilesDirectiveHandler();
  virtual bool isValid(const std::vector<std::string>& tokens) const;
  virtual void setConfig(ServerConfig& server_config);
};

#endif
