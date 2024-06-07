#include "./config/TryFilesDirectiveHandler.hpp"

TryFilesDirectiveHandler::TryFilesDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool TryFilesDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void TryFilesDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    if (Utils::isStartsWith(tokens_[i], "=")) {
      size_t status_code;
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
