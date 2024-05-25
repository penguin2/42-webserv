#include "./config/ClientMaxBodySizeDirectiveHandler.hpp"

ClientMaxBodySizeDirectiveHandler::ClientMaxBodySizeDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ClientMaxBodySizeDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }

  const std::string& size_string = tokens_[1];

  char lastChar = size_string.back();
  if (lastChar != 'M' && lastChar != 'm') {
    return false;
  }
  if (size_string.length() > 4) {
    std::cerr << "ClientMaxBodySize is too large" << tokens_[1] << std::endl;
    return false;
  }
  return true;
}

void ClientMaxBodySizeDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  if (!tokens_[1].empty()) {
    tokens_[1].pop_back();
  }
  size_t client_max_body_size_bytes;
  if (Utils::strToSize_t(tokens_[1], client_max_body_size_bytes, 10) == false) {
    std::cerr << "Failed to strToSize_t" << tokens_[1] << std::endl;
    exit(1);
  }
  size_t client_max_body_size_mega_bytes = (client_max_body_size_bytes * 1000000);
  locationConfig.setClientMaxBodySize(client_max_body_size_mega_bytes);
}
