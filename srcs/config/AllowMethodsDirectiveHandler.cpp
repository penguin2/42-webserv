#include "./config/AllowMethodsDirectiveHandler.hpp"

AllowMethodsDirectiveHandler::AllowMethodsDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool AllowMethodsDirectiveHandler::isValid() const {
  (void)tokens;
  return true;
}

void AllowMethodsDirectiveHandler::setConfig(long unsigned int server_num,
                                             std::string location_path) {
  LOG(DEBUG, "setting : ", this->tokens[0]);
  LOG(DEBUG, "server num : ", server_num);
  LOG(DEBUG, "location path : ", location_path);

  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num);
  if (!serverConfig.hasLocationConfig(location_path)) {
    LocationConfig* newLocationConfig = new LocationConfig();
    serverConfig.addLocationConfig(location_path, *newLocationConfig);
  }

  LocationConfig& locationConfig = serverConfig.getLocationConfig(location_path);

  std::vector<HttpMethod> allow_methods_vector;

  size_t i = 1;
  while (tokens[i] != ";") {
    if (tokens[i] == "GET") {
      allow_methods_vector.push_back(GET);
    } else if (tokens[i] == "POST") {
      allow_methods_vector.push_back(POST);
    } else if (tokens[i] == "DELETE") {
      allow_methods_vector.push_back(DELETE);
    } else {
      std::cerr << tokens[i] << " method does not exist" << std::endl;
      exit(1);
    }
    i++;
  }
  locationConfig.setAllowMethods(allow_methods_vector);
}
