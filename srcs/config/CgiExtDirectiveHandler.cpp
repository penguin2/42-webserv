#include "./config/CgiExtDirectiveHandler.hpp"

CgiExtDirectiveHandler::CgiExtDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiExtDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void CgiExtDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  for (size_t i = 1; i < this->tokens_.size() - 1; i++) {
    locationConfig.addCgiExt(tokens_[i]);
  }
}
