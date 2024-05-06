#include "./config/IndexDirectiveHandler.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void IndexDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path_);

  locationConfig.setIndex(tokens_[1]);
}
