#include "./config/ReturnDirectiveHandler.hpp"

ReturnDirectiveHandler::ReturnDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ReturnDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void ReturnDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    LocationConfig* newLocationConfig = new LocationConfig();
    serverConfig.addLocationConfig(location_path, *newLocationConfig);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;

  locationConfig.setReturnStatusCode(tokens[1]);
  locationConfig.setReturnUri(tokens[2]);
}
