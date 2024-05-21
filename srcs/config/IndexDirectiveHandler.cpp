#include "./config/IndexDirectiveHandler.hpp"

#include "UriUtils.hpp"
#include "Utils.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  const std::string& index_path = tokens_[1];
  if (!Utils::isContainsOnly(index_path,
                             UriUtils::isPathCharsetWithoutPctEncoding))
    return false;
  return true;
}

void IndexDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setIndex(tokens_[1]);
}
