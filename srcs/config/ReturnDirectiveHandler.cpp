#include "./config/ReturnDirectiveHandler.hpp"

ReturnDirectiveHandler::ReturnDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ReturnDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void ReturnDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  int return_status_code;
  if (!Utils::parseValue(tokens_[1], return_status_code)) {
    std::cout << "Failed to parse as int" << std::endl;
    exit(1);
  }

  locationConfig.setReturnStatusCode(return_status_code);

  locationConfig.setReturnUri(tokens_[2]);
}
