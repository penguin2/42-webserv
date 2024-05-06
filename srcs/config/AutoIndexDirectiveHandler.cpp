#include "./config/AutoIndexDirectiveHandler.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AutoIndexDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void AutoIndexDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path_);

  locationConfig.setAutoindex(tokens_[1] == "on");
}
