#ifndef WEBSERV_CONFIG_H
#define WEBSERV_CONFIG_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "ServerConfig.hpp"

class Config {
 private:
  Config();
  Config(const char* config_file);
  Config(const Config&);
  Config& operator=(const Config&);
  ~Config();

  std::vector<ServerConfig> servers;

 public:
  static Config& getInstance();

  void addServer(const ServerConfig& server);

  ServerConfig& getServer(long unsigned int server_num);

  const std::vector<ServerConfig>& getServers() const;

  void print();

  static bool isCorrespondingMethod(const std::string& method);
  static std::vector<std::string> makeAllMethods(void);
};

#endif
