#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool TryFilesDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void TryFilesDirectiveHandler::setConfig(long unsigned int server_num,
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
    if (tokens[i].empty()) {
      continue;
    }
    if (Utils::isStartsWith(tokens[i], "=")) {
      int status_code;
      std::string value = tokens[i].substr(1);
      if (!Utils::parseValue(value, status_code)) {
        std::cout << "Failed to parse as int" << std::endl;
        exit(1);
      }
      locationConfig.setTryFilesErrorCode(status_code);
    } else {
      locationConfig.addTryFilesPath(tokens[i]);
    }
  }
}