#include "./config/RootDirectiveHandler.hpp"

#include "utils/UriUtils.hpp"
#include "utils/Utils.hpp"

RootDirectiveHandler::RootDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool RootDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  const std::string& root = tokens_[1];
  if (!utils::isContainsOnly(root, uri_utils::isPathCharsetWithoutPctEncoding))
    return false;
  return true;
}

void RootDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setRoot(tokens_[1]);
}
