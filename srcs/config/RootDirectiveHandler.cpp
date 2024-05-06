#include "./config/RootDirectiveHandler.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool RootDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void RootDirectiveHandler::setConfig() {
  log();

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path_);

  locationConfig.setLocationRoot(tokens_[1]);
}