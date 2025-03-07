#include "./config/AutoIndexDirectiveHandler.hpp"

#include "utils/Utils.hpp"

AutoIndexDirectiveHandler::AutoIndexDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AutoIndexDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  std::string on_or_off = utils::toLower(tokens_[1]);
  if (on_or_off != "on" && on_or_off != "off") return false;
  return true;
}

void AutoIndexDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setAutoindex(utils::toLower(tokens_[1]) == "on");
}
