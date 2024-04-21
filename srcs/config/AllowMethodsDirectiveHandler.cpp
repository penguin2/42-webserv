#include "./config/AllowMethodsDirectiveHandler.hpp"

AllowMethodsDirectiveHandler::AllowMethodsDirectiveHandler() {
  this->directive_context_ = LOCATION;
}

bool AllowMethodsDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void AllowMethodsDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig serverConfig = config.getServer(server_num);
  LocationConfig locationConfig = serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;
}
