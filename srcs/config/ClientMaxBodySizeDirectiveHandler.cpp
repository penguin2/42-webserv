#include "./config/ClientMaxBodySizeDirectiveHandler.hpp"

ClientMaxBodySizeDirectiveHandler::ClientMaxBodySizeDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ClientMaxBodySizeDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void ClientMaxBodySizeDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path_);

  locationConfig.setClientMaxBodySize(tokens_[1]);
}
