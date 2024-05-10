#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void ServerNameDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  serverConfig.setServerName(this->tokens_[1]);
}