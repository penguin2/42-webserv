#ifndef ALLOW_METHODS_DIRECTIVE_HANDLER_HPP
#define ALLOW_METHODS_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class AllowMethodsDirectiveHandler : public ADirectiveHandler {
 public:
  AllowMethodsDirectiveHandler();
  virtual bool isValid(const std::vector<std::string>& tokens) const;
  virtual void setConfig(ServerConfig& server_config);
};

#endif
