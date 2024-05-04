#include "./config/ServerNameDirectiveHandler.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void ServerNameDirectiveHandler::setConfig(long unsigned int server_num,
                                           std::string location_path) {
  LOG(DEBUG, "setting : ", this->tokens[0]);
  LOG(DEBUG, "server num : ", server_num);
  LOG(DEBUG, "location path : ", location_path);

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    LocationConfig* newLocationConfig = new LocationConfig();
    serverConfig.addLocationConfig(location_path, *newLocationConfig);
  }

  serverConfig.setServerName(this->tokens[1]);
}