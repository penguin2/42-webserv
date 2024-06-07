#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "LocationConfig.hpp"

class ServerConfig {
 private:
  std::string server_name;
  std::string listen_address;
  std::string listen_port;
  std::map<size_t, std::string> error_pages;
  std::map<std::string, LocationConfig> location_configs;

 public:
  ServerConfig();
  ServerConfig(const ServerConfig&);
  ServerConfig& operator=(const ServerConfig&);

  void setServerName(const std::string& server_name);

  void setListenAddress(const std::string& listen_address);

  void setListenPort(const std::string& listen_port);

  void addErrorPage(size_t status_code, const std::string& error_page);

  void addLocationConfig(const std::string& location_path);

  const std::string& getServerName() const;

  const std::string& getListenAddress() const;

  const std::string& getListenPort() const;

  const std::string& getErrorPage(size_t status_code) const;

  LocationConfig& getLocationConfig(const std::string& location_path);

  const std::map<std::string, LocationConfig>& getLocationConfigs() const;

  bool hasLocationConfig(const std::string& location_path) const;

  void print();
};

// for debug
// std::ostream& operator<<(std::ostream& os, const ServerConfig& serverConfig)
// {
//   os << "Server Name: " << serverConfig.getServerName() << std::endl;
//   os << "Listen Address: " << serverConfig.getListenAddress() << std::endl;
//   os << "Error Pages:" << std::endl;
//   for (std::map<int, std::string>::iterator it = this->error_pages.begin();
//        it != this->error_pages.end(); ++it) {
//     os << "status_code: " << it->first << ", error_page: " << it->second
//        << std::endl;
//   }

//   for (std::map<std::string, LocationConfig>::iterator it =
//            this->location_configs.begin();
//        it != this->location_configs.end(); ++it) {
//     os << "      ********** location **********" << std::endl;
//     os << "  path: " << it->first << std::endl;
//     it->second.print();
//   }
//   return os;
// }

#endif
