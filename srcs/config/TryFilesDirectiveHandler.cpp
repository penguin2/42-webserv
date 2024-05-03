#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool TryFilesDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

template <typename T>
bool parseValue(const std::string& str, T& value) {
  std::istringstream ss(str);
  return !(ss >> value).fail() && ss.eof();
}

void TryFilesDirectiveHandler::setConfig(long unsigned int server_num,
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
    if (tokens[i].empty()) {
      continue;
    }
    if (tokens[i][0] == '=') {
      int status_code;
      std::string value = tokens[i].substr(1);
      if (!parseValue(value, status_code)) {
        std::cout << "Failed to parse as int" << std::endl;
        exit(1);
      }
      locationConfig.setTryFilesErrorCode(status_code);
    } else {
      locationConfig.addTryFilesPath(tokens[i]);
    }
  }
}