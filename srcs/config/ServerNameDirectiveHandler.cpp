#include "./config/ServerNameDirectiveHandler.hpp"

bool ServerNameDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;  
  return true;
}
