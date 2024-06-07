#include "./config/ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& other) {
  this->server_name = other.server_name;
  this->listen_address = other.listen_address;
  this->listen_port = other.listen_port;
  this->error_pages = other.error_pages;
  this->location_configs = other.location_configs;
}

ServerConfig& ServerConfig::operator=(const ServerConfig&) { return *this; }

void ServerConfig::setServerName(const std::string& server_name) {
  this->server_name = server_name;
}

void ServerConfig::setListenAddress(const std::string& listen_address) {
  this->listen_address = listen_address;
}

void ServerConfig::setListenPort(const std::string& listen_port) {
  this->listen_port = listen_port;
}

void ServerConfig::addErrorPage(size_t status_code,
                                const std::string& error_page) {
  this->error_pages[status_code] = error_page;
}

void ServerConfig::addLocationConfig(const std::string& location_path) {
  this->location_configs[location_path] = LocationConfig();
}

const std::string& ServerConfig::getServerName() const { return server_name; }

const std::string& ServerConfig::getListenAddress() const {
  return listen_address;
}

const std::string& ServerConfig::getListenPort() const { return listen_port; }

const std::string& ServerConfig::getErrorPage(size_t error_code) const {
  std::map<size_t, std::string>::const_iterator it =
      error_pages.find(error_code);
  if (it != error_pages.end()) {
    return it->second;
  }
  static const std::string default_error_page = "";
  return default_error_page;
}

LocationConfig& ServerConfig::getLocationConfig(
    const std::string& location_path) {
  std::map<std::string, LocationConfig>::iterator it =
      location_configs.find(location_path);
  if (it != location_configs.end()) {
    return it->second;
  }
  // 指定された場所のLocationConfigが見つからなかった場合、新しいLocationConfigを作成してマップに追加する
  static LocationConfig default_location_config;
  return default_location_config;
}

const std::map<std::string, LocationConfig>& ServerConfig::getLocationConfigs()
    const {
  return location_configs;
}

bool ServerConfig::hasLocationConfig(const std::string& location_path) const {
  if (location_path == "") {
    return true;
  }
  return location_configs.find(location_path) != location_configs.end();
}

void ServerConfig::print() {
  std::cout << "server_name: " << this->server_name << std::endl;
  std::cout << "listen_address: " << this->listen_address << std::endl;

  for (std::map<size_t, std::string>::iterator it = this->error_pages.begin();
       it != this->error_pages.end(); ++it) {
    std::cout << "error_code: " << it->first << ", error_page: " << it->second
              << std::endl;
  }

  for (std::map<std::string, LocationConfig>::iterator it =
           this->location_configs.begin();
       it != this->location_configs.end(); ++it) {
    std::cout << "      ********** location **********" << std::endl;
    std::cout << "  path: " << it->first << std::endl;
    it->second.print();
  }
}
