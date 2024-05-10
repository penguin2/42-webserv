#include "./config/IndexDirectiveHandler.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void IndexDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setIndex(tokens_[1]);
}
