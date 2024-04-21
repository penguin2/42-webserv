#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = SERVER;
}

bool ServerNameDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  (void)tokens;  
  return true;
}

void ServerNameDirectiveHandler::setConfig(ServerConfig& server_config) { (void)server_config; }
