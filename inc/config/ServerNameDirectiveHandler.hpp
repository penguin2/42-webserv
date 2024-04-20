#ifndef SERVER_NAME_DIRECTIVE_HANDLER_HPP
#define SERVER_NAME_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ServerNameDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
