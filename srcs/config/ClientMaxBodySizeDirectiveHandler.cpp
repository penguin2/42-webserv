#include "./config/ClientMaxBodySizeDirectiveHandler.hpp"

ClientMaxBodySizeDirectiveHandler::ClientMaxBodySizeDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ClientMaxBodySizeDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void ClientMaxBodySizeDirectiveHandler::setConfig(long unsigned int server_num,
                                                  std::string location_path) {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path);

  locationConfig.setClientMaxBodySize(tokens_[1]);
}
