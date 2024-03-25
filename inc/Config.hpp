#ifndef WEBSERV_CONFIG_H
#define WEBSERV_CONFIG_H

#include <memory>
#include <vector>

#include "ServerConfig.hpp"

class Config {
 private:
  Config() {}
  Config(const char* config_file) {}
  Config(const Config&);
  Config& operator=(const Config&);
  ~Config() {}

  std::vector<ServerConfig> servers;

 public:
  static Config& getInstance(const char* config_file) {
    static Config instance(config_file);
    return instance;
  }

  void addServer(const ServerConfig& server) {}

  const std::vector<ServerConfig>& getServers() const {}
};

#endif
