#include "./config/ListenDirectiveHandler.hpp"

ListenDirectiveHandler::ListenDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ListenDirectiveHandler::isValid() const {
  if (tokens.size() < 3) {
    return false;
  }
  return true;
}

void ListenDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig serverConfig = config.getServer(server_num);
  LocationConfig locationConfig = serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;
}