#include "./config/CgiExtDirectiveHandler.hpp"

CgiExtDirectiveHandler::CgiExtDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiExtDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void CgiExtDirectiveHandler::setConfig(long unsigned int server_num,
                                       std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    LocationConfig* newLocationConfig = new LocationConfig();
    serverConfig.addLocationConfig(location_path, *newLocationConfig);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;
  for (size_t i = 1; i < this->tokens.size() - 1; i++) {
    locationConfig.addCgiExt(tokens[i]);
  }
}
