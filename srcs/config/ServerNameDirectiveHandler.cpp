#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void ServerNameDirectiveHandler::setConfig() {
  log();

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  serverConfig.setServerName(this->tokens_[1]);
}