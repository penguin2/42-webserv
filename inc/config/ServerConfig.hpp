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
  size_t listen_port;
  std::map<int, std::string> error_pages;
  std::map<std::string, LocationConfig> location_configs;

 public:
  ServerConfig();
  ServerConfig(const ServerConfig&);
  ServerConfig& operator=(const ServerConfig&);

  void setServerName(const std::string& server_name);

  void setListenAddress(const std::string& listen_address);

  void setListenPort(const size_t listen_port);

  void addErrorPage(int error_code, const std::string& error_page);

  void addLocationConfig(const std::string& location_path,
                         const LocationConfig& location_config);

  const std::string& getServerName() const;

  const std::string& getListenAddress() const;

  const std::string& getErrorPage(int error_code) const;

  LocationConfig& getLocationConfig(const std::string& location_path);

  const std::map<std::string, LocationConfig>& getLocationConfigs() const;

  bool hasLocationConfig(const std::string& location_path) const;

  void print();
};

// for debug
//  std::ostream& operator<<(std::ostream& os, const ServerConfig& serverConfig)
//  {
//      os << "Server Name: " << serverConfig.getServerName() << std::endl;
//      os << "Listen Address: " << serverConfig.getListenAddress() <<
//      std::endl;

//     os << "Error Pages:" << std::endl;
//     // const std::map<int, std::string>& errorPages =
//     serverConfig.getErrorPages();
//     // for (std::map<int, std::string>::const_iterator it =
//     errorPages.begin(); it != errorPages.end(); ++it) {
//     //     os << "  " << it->first << ": " << it->second << std::endl;
//     // }

//     os << "Location Configurations:" << std::endl;
//     const std::map<std::string, LocationConfig>& locationConfigs =
//     serverConfig.getLocationConfigs(); for (std::map<std::string,
//     LocationConfig>::const_iterator it = locationConfigs.begin(); it !=
//     locationConfigs.end(); ++it) {
//         os << "  Location Path: " << it->first << std::endl;
//         // os << "  Location Config: " << it->second << std::endl;
//     }
//     return os;
// }

#endif
