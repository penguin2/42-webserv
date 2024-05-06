#include "./config/IndexDirectiveHandler.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void IndexDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path);

  locationConfig.setIndex(tokens_[1]);
}
