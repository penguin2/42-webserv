#include "./config/ServerNameDirectiveHandler.hpp"

#include "UriUtils.hpp"
#include "Utils.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }

  std::string servername = tokens_[1];
  return Utils::isContainsOnly(servername,
                               UriUtils::isRegNameWithoutPctEncoding);
}

void ServerNameDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  serverConfig.setServerName(Utils::toLower(this->tokens_[1]));
}
