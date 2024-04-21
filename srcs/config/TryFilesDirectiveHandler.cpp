#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool TryFilesDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;
  return true;
}

void TryFilesDirectiveHandler::setConfig(ServerConfig& server_config) { (void)server_config; }
