#ifndef WEBSERV_CONFIG_H
#define WEBSERV_CONFIG_H

#include <vector>
#include <memory>
#include "ServerConfig.hpp"

class Config {
private:
    Config(const char* config_file) {}
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    std::vector<Server> servers;

public:
    static Config& getInstance(const char* config_file) {
        static Config instance(config_file);
        return instance;
    }

    void addServer(const Server& server) {
        servers.push_back(server);
    }

    const std::vector<Server>& getServers() const {
        return servers;
    }  
};

#endif
