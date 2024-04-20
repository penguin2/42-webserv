#include "./config/ErrorPageDirectiveHandler.hpp"

ErrorPageDirectiveHandler::ErrorPageDirectiveHandler() {
  this->directive_context_ = SERVER;
}

bool ErrorPageDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}
