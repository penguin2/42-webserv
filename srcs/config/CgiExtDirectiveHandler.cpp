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
  LOG(DEBUG, "setting : ", this->tokens[0]);
  LOG(DEBUG, "server num : ", server_num);
  LOG(DEBUG, "location path : ", location_path);

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path);

  for (size_t i = 1; i < this->tokens.size() - 1; i++) {
    locationConfig.addCgiExt(tokens[i]);
  }
}
