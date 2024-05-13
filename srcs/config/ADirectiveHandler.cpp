#include "./config/ADirectiveHandler.hpp"

ADirectiveHandler::ADirectiveHandler() {}

ADirectiveHandler::~ADirectiveHandler() {}

bool ADirectiveHandler::isSyntaxValid() const {
  int semicolonCount =
      std::count(this->tokens_.begin(), this->tokens_.end(), ";");
  if (semicolonCount != 1) {
    return false;
  }
  return this->tokens_.size() >= 2 && this->tokens_.back() == ";";
}

bool ADirectiveHandler::isMatchContext(ConfigEnums::Context context) {
  return directive_context_ == context;
}

void ADirectiveHandler::setToken(const std::vector<std::string>& tokens) {
  this->tokens_ = tokens;
}

void ADirectiveHandler::setServerNum(long unsigned int server_num) {
  this->server_num_ = server_num;
}

void ADirectiveHandler::setLocationPath(std::string location_path) {
  this->location_path_ = location_path;
}

void ADirectiveHandler::log() {
  LOG(DEBUG, "setting : ", this->tokens_[0]);
  LOG(DEBUG, "server num : ", this->server_num_);
  LOG(DEBUG, "location path : ", this->location_path_);
}

ServerConfig& ADirectiveHandler::getServerConfig() {
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }
  return serverConfig;
}

LocationConfig& ADirectiveHandler::getLocationConfig() {
  Config& config = Config::getInstance();
  ServerConfig& serverConfig = config.getServer(server_num_);
  if (!serverConfig.hasLocationConfig(location_path_)) {
    serverConfig.addLocationConfig(location_path_);
  }
  return serverConfig.getLocationConfig(location_path_);
}
