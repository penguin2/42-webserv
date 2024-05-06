#include "./config/CgiExtDirectiveHandler.hpp"

CgiExtDirectiveHandler::CgiExtDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiExtDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void CgiExtDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path_);

  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    locationConfig.addCgiExt(tokens_[i]);
  }
}
