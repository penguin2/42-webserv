#include "./config/IndexDirectiveHandler.hpp"

#include "utils/UriUtils.hpp"
#include "utils/Utils.hpp"

IndexDirectiveHandler::IndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool IndexDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  const std::string& index_path = tokens_[1];
  if (!utils::isContainsOnly(index_path,
                             uri_utils::isPathCharsetWithoutPctEncoding))
    return false;
  return true;
}

void IndexDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setIndex(tokens_[1]);
}
