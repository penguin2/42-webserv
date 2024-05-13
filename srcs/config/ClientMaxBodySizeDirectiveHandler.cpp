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
  if (!tokens_[1].empty()) {
    tokens_[1].pop_back();
  }
  size_t num;
  if (Utils::strToSize_t(tokens_[1], num, 10) == false) {
    std::cerr << "Failed to strToSize_t" << tokens_[1] << std::endl;
    exit(1);
  }
  locationConfig.setClientMaxBodySize(num);
}
