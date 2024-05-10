#include "./config/RootDirectiveHandler.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool RootDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void RootDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setRoot(tokens_[1]);
}