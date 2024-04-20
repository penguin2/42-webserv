#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = SERVER;
}

bool ServerNameDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;  
  return true;
}
