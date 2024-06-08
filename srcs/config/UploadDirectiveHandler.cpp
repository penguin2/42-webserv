#include "./config/UploadDirectiveHandler.hpp"

#include "Utils.hpp"

UploadDirectiveHandler::UploadDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool UploadDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  std::string on_or_off = Utils::toLower(tokens_[1]);
  if (on_or_off != "on" && on_or_off != "off") return false;
  return true;
}

void UploadDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  locationConfig.setAllowUpload(Utils::toLower(tokens_[1]) == "on");
}
