#include "./config/IndexDirectiveHandler.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void IndexDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  LOG(DEBUG, "setting : ", this->tokens[0]);
  LOG(DEBUG, "server num : ", server_num);
  LOG(DEBUG, "location path : ", location_path);

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path);

  locationConfig.setIndex(tokens[1]);
}
