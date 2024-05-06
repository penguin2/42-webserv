#include "./config/AutoIndexDirectiveHandler.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AutoIndexDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void AutoIndexDirectiveHandler::setConfig(long unsigned int server_num,
                                          std::string location_path) {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path);

  locationConfig.setAutoindex(tokens_[1] == "on");
}
