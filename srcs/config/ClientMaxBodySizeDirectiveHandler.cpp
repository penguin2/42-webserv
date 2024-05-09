#include "./config/ClientMaxBodySizeDirectiveHandler.hpp"

ClientMaxBodySizeDirectiveHandler::ClientMaxBodySizeDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ClientMaxBodySizeDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void ClientMaxBodySizeDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setClientMaxBodySize(tokens_[1]);
}
