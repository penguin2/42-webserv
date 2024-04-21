#include "./config/AllowMethodsDirectiveHandler.hpp"

AllowMethodsDirectiveHandler::AllowMethodsDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool AllowMethodsDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}

void AllowMethodsDirectiveHandler::setConfig(ServerConfig& server_config) {
  (void)server_config;
}
