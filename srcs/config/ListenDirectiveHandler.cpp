#include "./config/ListenDirectiveHandler.hpp"

ListenDirectiveHandler::ListenDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ListenDirectiveHandler::isValid() const {
  if (tokens_.size() < 3) {
    return false;
  }
  return true;
}

void ListenDirectiveHandler::parseAddressAndPort(const std::string& input,
                                                 std::string& address,
                                                 int& port) {
  std::stringstream ss(input);
  std::getline(ss, address, ':');
  ss >> port;
}

void ListenDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  std::string address;
  int port;

  parseAddressAndPort(tokens_[1], address, port);

  serverConfig.setListenAddress(address);
  serverConfig.setListenPort(port);
}