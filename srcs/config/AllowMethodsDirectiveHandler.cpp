#include "./config/AllowMethodsDirectiveHandler.hpp"

AllowMethodsDirectiveHandler::AllowMethodsDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AllowMethodsDirectiveHandler::isDirectiveValid() const {
  std::map<std::string, int> methodCounts;
  std::vector<std::string> all_methods = Config::makeAllMethods();

  if (tokens_.size() < 3) {
    return false;
  }
  for (size_t i = 1; i < tokens_.size(); ++i) {
    if (i == tokens_.size() - 1) {
      break;
    }
    methodCounts[tokens_[i]]++;
  }

  for (std::map<std::string, int>::const_iterator it = methodCounts.begin();
       it != methodCounts.end(); ++it) {
    if (!Config::isCorrespondingMethod(it->first)) {
      return false;
    } else {
      if (it->second > 1) {
        return false;
      }
    }
  }
  return true;
}

void AllowMethodsDirectiveHandler::setConfig() {
  log();

  LocationConfig& locationConfig = getLocationConfig();

  std::vector<std::string> allow_methods_vector;

  size_t i = 1;
  while (tokens_[i] != ";") {
    allow_methods_vector.push_back(tokens_[i]);
    i++;
  }
  locationConfig.setAllowMethods(allow_methods_vector);
}
