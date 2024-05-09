#include "./config/AutoIndexDirectiveHandler.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AutoIndexDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void AutoIndexDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setAutoindex(tokens_[1] == "on");
}
