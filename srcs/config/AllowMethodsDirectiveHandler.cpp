#include "./config/AllowMethodsDirectiveHandler.hpp"

AllowMethodsDirectiveHandler::AllowMethodsDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AllowMethodsDirectiveHandler::isDirectiveValid() const {
  (void)tokens_;
  return true;
}

void AllowMethodsDirectiveHandler::setConfig() {
  log();

  LocationConfig& locationConfig = getLocationConfig();

  std::vector<HttpMethod> allow_methods_vector;

  size_t i = 1;
  while (tokens_[i] != ";") {
    if (tokens_[i] == "GET") {
      allow_methods_vector.push_back(GET);
    } else if (tokens_[i] == "POST") {
      allow_methods_vector.push_back(POST);
    } else if (tokens_[i] == "DELETE") {
      allow_methods_vector.push_back(DELETE);
    } else {
      std::cerr << tokens_[i] << " method does not exist" << std::endl;
      exit(1);
    }
    i++;
  }
  locationConfig.setAllowMethods(allow_methods_vector);
}
