#include "./config/CgiPathDirectiveHandler.hpp"

CgiPathDirectiveHandler::CgiPathDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiPathDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void CgiPathDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path_);

  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    locationConfig.addCgiPath(tokens_[i]);
  }
}
