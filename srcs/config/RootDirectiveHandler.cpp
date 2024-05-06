#include "./config/RootDirectiveHandler.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool RootDirectiveHandler::isValid() const {
  (void)tokens_;
  return true;
}

void RootDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setLocationRoot(tokens_[1]);
}