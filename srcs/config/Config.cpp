#include "./config/Config.hpp"

Config::Config() {}

// Config::Config(const char* config_file) { (void)config_file; }

// Config::Config(const Config&) {}

// Config& Config::operator=(const Config&) { return *this; }

Config::~Config() {}

Config& Config::getInstance() {
  static Config instance;
  return instance;
}

void Config::addServer(const ServerConfig& server) {
  servers.push_back(server);
}

ServerConfig& Config::getServer(long unsigned int server_num) {
  while (server_num >= servers.size()) {
    // 新しいサーバーコンフィグを作成して追加
    addServer(ServerConfig());
  }
  return servers[server_num];
}

const std::vector<ServerConfig>& Config::getServers() const {
  return servers;
}
