#include "./config/ListenDirectiveHandler.hpp"

ListenDirectiveHandler::ListenDirectiveHandler() {
  this->directive_context_ = SERVER;
}

bool ListenDirectiveHandler::isValid(
    const std::vector<std::string>& tokens) const {
  if (tokens.size() < 3) {
    return false;
  }
  return true;
}

void ListenDirectiveHandler::setConfig(ServerConfig& server_config) { (void)server_config; }
