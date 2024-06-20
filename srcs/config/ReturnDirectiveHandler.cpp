#include "./config/ReturnDirectiveHandler.hpp"

#include "HttpException.hpp"
#include "Uri.hpp"
#include "utils/HttpUtils.hpp"

ReturnDirectiveHandler::ReturnDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool ReturnDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 4) {
    return false;
  }

  size_t status_code;
  const std::string& status_code_string = tokens_[1];
  const std::string& uri_string = tokens_[2];

  try {
    Uri uri;
    uri.parse(uri_string);
  } catch (HttpException& e) {
    return false;
  }

  if (utils::strToSize_t(status_code_string, status_code, 10) == false ||
      !http_utils::isRedirectStatusCode(status_code))
    return false;
  return true;
}

void ReturnDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  size_t return_status_code;
  utils::strToSize_t(tokens_[1], return_status_code, 10);

  locationConfig.setReturnStatusCode(return_status_code);
  locationConfig.setReturnUri(tokens_[2]);
}
