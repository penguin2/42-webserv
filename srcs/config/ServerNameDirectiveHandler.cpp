#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void ServerNameDirectiveHandler::setConfig(long unsigned int server_num,
                                           std::string location_path) {
  log();

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  serverConfig.setServerName(this->tokens_[1]);
}