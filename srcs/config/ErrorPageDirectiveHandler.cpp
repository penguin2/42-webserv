#include "./config/ErrorPageDirectiveHandler.hpp"

ErrorPageDirectiveHandler::ErrorPageDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ErrorPageDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

template <typename T>
bool parseValue(const std::string& str, T& value) {
  std::istringstream ss(str);
  return !(ss >> value).fail() && ss.eof();
}

void ErrorPageDirectiveHandler::setConfig(long unsigned int server_num,
                                          std::string location_path) {
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    LocationConfig* newLocationConfig = new LocationConfig();
    serverConfig.addLocationConfig(location_path, *newLocationConfig);
  }

  size_t i = 1;
  int error_code;
  std::string error_page = tokens[tokens.size() - 2];
  while (i < tokens.size() - 2) {
    if (parseValue(tokens[i], error_code)) {
      std::cout << "Parsed as int: " << i << std::endl;
      serverConfig.addErrorPage(error_code, error_page);
    } else {
      std::cout << "Failed to parse as int" << std::endl;
      exit(1);
    }
    i++;
  }

  // LocationConfig& locationConfig =
  // serverConfig.getLocationConfig(location_path);
  std::cout << "setting : " << this->tokens[0] << std::endl;
  std::cout << "server num : " << server_num << std::endl;
  std::cout << "location path : " << location_path << std::endl;
}