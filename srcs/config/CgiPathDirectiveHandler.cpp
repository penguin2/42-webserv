#include "./config/CgiPathDirectiveHandler.hpp"

CgiPathDirectiveHandler::CgiPathDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiPathDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void CgiPathDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    locationConfig.addCgiPath(tokens_[i]);
  }
}
