#include "./config/ListenDirectiveHandler.hpp"

#include "UriUtils.hpp"

ListenDirectiveHandler::ListenDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ListenDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() != 3) {
    return false;
  }
  const std::string& socket_address = tokens_[1];
  std::string tmp_address;
  std::string tmp_port;
  return parseAddressAndPort(socket_address, tmp_address, tmp_port);
}

bool ListenDirectiveHandler::parseAddressAndPort(const std::string& str,
                                                 std::string& address,
                                                 std::string& port) {
  std::string port_raw_str;
  size_t port_number;
  const size_t pos_colon = str.find(":");

  if (pos_colon == std::string::npos) {
    address = "0.0.0.0";
    port_raw_str = str;
  } else {
    address = str.substr(0, pos_colon);
    if (Utils::toLower(address) == "localhost") address = "127.0.0.1";
    port_raw_str = str.substr(pos_colon + 1);
  }

  if (!UriUtils::isIPv4Address(address)) return false;
  if (Utils::strToSize_t(port_raw_str, port_number, 10) == false) return false;
  if (port_number < 1 || 65535 < port_number) return false;

  port = Utils::uintToString(port_number);
  return true;
}

void ListenDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  std::string address;
  std::string port;

  parseAddressAndPort(tokens_[1], address, port);

  serverConfig.setListenAddress(address);
  serverConfig.setListenPort(port);
}
