#include "./config/AutoIndexDirectiveHandler.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AutoIndexDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void AutoIndexDirectiveHandler::setConfig(long unsigned int server_num,
                                          std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig serverConfig = config.getServer(server_num);
  LocationConfig locationConfig = serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;
}
