#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <memory>
#include <string>
#include <vector>

#include "LocationConfig.hpp"

class ServerConfig {
 private:
  std::string server_name;
  std::string listen_address;
  std::map<int, std::string> error_pages;
  std::map<std::string, LocationConfig> location_configs;

 public:
  ServerConfig() {}
  ServerConfig(const ServerConfig&);
  ServerConfig& operator=(const ServerConfig&);

  void setServerName(const std::string& server_name) {}

  void setListenAddress(const std::string& listen_address) {}

  void addErrorPage(int error_code, const std::string& error_page) {}

  void addLocationConfig(const std::string& location_path,
                         const LocationConfig& location_config) {}

  const std::string& getServerName() const {}

  const std::string& getListenAddress() const {}

  const std::string& getErrorPage(int error_code) const {}

  const std::map<std::string, LocationConfig>& getLocationConfigs() const {}

  const LocationConfig& getLocationConfig() {}
};

#endif
