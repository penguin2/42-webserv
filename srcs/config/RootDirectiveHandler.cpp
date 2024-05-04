#include "./config/RootDirectiveHandler.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool RootDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void RootDirectiveHandler::setConfig(long unsigned int server_num,
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

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path);

  locationConfig.setLocationRoot(tokens[1]);
}