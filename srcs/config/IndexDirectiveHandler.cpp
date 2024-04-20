#include "./config/IndexDirectiveHandler.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool IndexDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}
