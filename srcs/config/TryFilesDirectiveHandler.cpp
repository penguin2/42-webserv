#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool TryFilesDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void TryFilesDirectiveHandler::setConfig(long unsigned int server_num,
                                         std::string location_path) {
  log();

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    serverConfig.addLocationConfig(location_path);
  }

  LocationConfig& locationConfig =
      serverConfig.getLocationConfig(location_path);

  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    if (tokens_[i].empty()) {
      continue;
    }
    if (Utils::isStartsWith(tokens_[i], "=")) {
      int status_code;
      std::string value = tokens_[i].substr(1);
      if (!Utils::parseValue(value, status_code)) {
        std::cout << "Failed to parse as int" << std::endl;
        exit(1);
      }
      locationConfig.setTryFilesErrorCode(status_code);
    } else {
      locationConfig.addTryFilesPath(tokens_[i]);
    }
  }
}