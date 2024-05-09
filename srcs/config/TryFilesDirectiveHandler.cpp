#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool TryFilesDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void TryFilesDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    if (Utils::isStartsWith(tokens_[i], "=")) {
      int status_code;
      if (tokens_[i].size() < 1) {
        std::cout << "Status code is required after =" << std::endl;
        exit(1);
      }
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