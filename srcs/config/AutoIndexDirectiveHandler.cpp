#include "./config/AutoIndexDirectiveHandler.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool AutoIndexDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}

void AutoIndexDirectiveHandler::setConfig(ServerConfig& server_config) { (void)server_config; }
