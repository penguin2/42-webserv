#include "./config/ReturnDirectiveHandler.hpp"

ReturnDirectiveHandler::ReturnDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ReturnDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

template <typename T>
bool parseValue(const std::string& str, T& value) {
  std::istringstream ss(str);
  return !(ss >> value).fail() && ss.eof();
}

void ReturnDirectiveHandler::setConfig(long unsigned int server_num,
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

  int return_status_code;
  if (!parseValue(tokens[1], return_status_code)) {
    std::cout << "Failed to parse as int" << std::endl;
    exit(1);
  }

  locationConfig.setReturnStatusCode(return_status_code);

  locationConfig.setReturnUri(tokens[2]);
}
