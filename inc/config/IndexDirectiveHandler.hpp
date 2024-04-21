#ifndef INDEX_DIRECTIVE_HANDLER_HPP
#define INDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class IndexDirectiveHandler : public ADirectiveHandler {
 public:
  IndexDirectiveHandler();
  virtual bool isValid(const std::vector<std::string>& tokens) const;
  virtual void setConfig(ServerConfig& server_config);
};

#endif
