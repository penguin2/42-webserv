#include "./config/RootDirectiveHandler.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool RootDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}
