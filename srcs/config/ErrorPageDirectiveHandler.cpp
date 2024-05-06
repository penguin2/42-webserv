#include "./config/ErrorPageDirectiveHandler.hpp"

ErrorPageDirectiveHandler::ErrorPageDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ErrorPageDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void ErrorPageDirectiveHandler::setConfig() {
  log();
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }

  size_t i = 1;
  int error_code;
  std::string error_page = tokens_[tokens_.size() - 2];
  while (i < tokens_.size() - 2) {
    if (Utils::parseValue(tokens_[i], error_code)) {
      LOG(DEBUG, "Parsed as int: ", i);
      serverConfig.addErrorPage(error_code, error_page);
    } else {
      std::cout << "Failed to parse as int" << std::endl;
      exit(1);
    }
    i++;
  }
}