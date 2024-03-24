#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <memory>
#include "LocationConfig.hpp"

class ServerConfig {
private:
    std::string server_name;
    std::string listen_address;
    std::map<int, std::string> error_pages;
    std::map<std::string, LocationConfig> location_configs;

public:
    ServerConfig(){}
    ServerConfig(const ServerConfig&);
    ServerConfig& operator=(const ServerConfig&);

    void setServerName(const std::string& server_name) {
        this->server_name = server_name;
    }

    void setListenAddress(const std::string& listen_address) {
        this->listen_address = listen_address;
    }

    void addErrorPage(int error_code, const std::string& error_page) {
        error_pages[error_code] = error_page;
    }

    void addLocationConfig(const std::string& location_path, const LocationConfig& location_config) {
        location_configs[location_path] = location_config;
    }

    const std::string& getServerName() const {
        return server_name;
    }

    const std::string& getListenAddress() const {
        return listen_address;
    }

    const std::string& getErrorPage(int error_code) const {
        auto it = error_pages.find(error_code);
        if (it != error_pages.end()) {
            return it->second;
        } else {
            static const std::string empty_string;
            return empty_string;
        }
    }

    const std::map<std::string, LocationConfig>& getLocationConfigs() const {
        return location_configs;
    }

    const LocationConfig& getLocationConfig(const std::string& location_path) const {
    auto it = location_configs.find(location_path);
    if (it != location_configs.end()) {
        return it->second;
    } else {
        throw std::out_of_range("Location not found for path: " + location_path);
    }
}
};

#endif
