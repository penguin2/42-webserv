#include "./config/Config.hpp"

Config::Config() {}

Config::Config(const char* config_file) { (void)config_file; }

Config::Config(const Config&) {}

Config& Config::operator=(const Config&) { return *this; }

Config::~Config() {}

Config& Config::getInstance(const char* config_file) {
  static Config instance(config_file);
  return instance;
}

void Config::addServer(const ServerConfig& server) {
  servers.push_back(server);
}

const std::vector<ServerConfig>& Config::getServers() const {
  return servers;
}
