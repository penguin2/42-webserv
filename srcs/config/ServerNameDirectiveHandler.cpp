#include "./config/ServerNameDirectiveHandler.hpp"

#include "utils/UriUtils.hpp"
#include "utils/Utils.hpp"

ServerNameDirectiveHandler::ServerNameDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ServerNameDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }

  std::string servername = tokens_[1];
  return utils::isContainsOnly(servername,
                               uri_utils::isRegNameWithoutPctEncoding);
}

void ServerNameDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  serverConfig.setServerName(utils::toLower(this->tokens_[1]));
}
