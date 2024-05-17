#include "./config/Config.hpp"

#include <algorithm>

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

ServerConfig& Config::getServer(long unsigned int server_index) {
  while (server_index > servers.size()) {
    // 新しいサーバーコンフィグを作成して追加
    addServer(ServerConfig());
  }
  if (server_index < 1) {
    std::cerr << "server count is invalid" << std::endl;
    exit(1);
  }
  return servers[server_index - 1];
}

const std::vector<ServerConfig>& Config::getServers() const { return servers; }

void Config::print() {
  for (std::vector<ServerConfig>::iterator it = servers.begin();
       it != servers.end(); ++it) {
    std::cout << "********** server **********" << std::endl;
    it->print();
  }
}

bool Config::isCorrespondingMethod(const std::string& method) {
  static const std::vector<std::string> all_methods = Config::makeAllMethods();

  return (std::find(all_methods.begin(), all_methods.end(), method) !=
          all_methods.end());
}

std::vector<std::string> Config::makeAllMethods(void) {
  std::vector<std::string> all_methods;

  all_methods.push_back("GET");
  all_methods.push_back("POST");
  all_methods.push_back("DELETE");
  return all_methods;
}
