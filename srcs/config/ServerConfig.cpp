#include "./config/ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig&) {}

ServerConfig& ServerConfig::operator=(const ServerConfig&) { return *this; }

void ServerConfig::setServerName(const std::string& server_name) {
  this->server_name = server_name;
}

void ServerConfig::setListenAddress(const std::string& listen_address) {
  this->listen_address = listen_address;
}

void ServerConfig::addErrorPage(int error_code, const std::string& error_page) {
  this->error_pages[error_code] = error_page;
}

void ServerConfig::addLocationConfig(const std::string& location_path,
                                     const LocationConfig& location_config) {
  this->location_configs[location_path] = location_config;
}

const std::string& ServerConfig::getServerName() const { return server_name; }

const std::string& ServerConfig::getListenAddress() const {
  return listen_address;
}

const std::string& ServerConfig::getErrorPage(int error_code) const {
  std::map<int, std::string>::const_iterator it = error_pages.find(error_code);
  if (it != error_pages.end()) {
    return it->second;
  }
  static const std::string default_error_page = "";
  return default_error_page;
}

const LocationConfig& ServerConfig::getLocationConfig(
    const std::string& location_path) {
  std::map<std::string, LocationConfig>::iterator it =
      location_configs.find(location_path);
  if (it != location_configs.end()) {
    return it->second;
  }
  // 指定された場所のLocationConfigが見つからなかった場合、新しいLocationConfigを作成してマップに追加する
  static const LocationConfig default_location_config;
  return default_location_config;
}

const std::map<std::string, LocationConfig>& ServerConfig::getLocationConfigs()
    const {
  return location_configs;
}

bool ServerConfig::hasLocationConfig(const std::string& location_path) const {
  return location_configs.find(location_path) != location_configs.end();
}
