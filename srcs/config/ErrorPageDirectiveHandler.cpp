#include "./config/ErrorPageDirectiveHandler.hpp"

bool ErrorPageDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}
