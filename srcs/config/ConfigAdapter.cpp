#include "config/ConfigAdapter.hpp"

#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

const ServerConfig &ConfigAdapter::searchServerConfig(const std::string &host,
                                                      size_t port) {
  return Config::getInstance().getServer(0);
  (void)host;
  (void)port;
}

const LocationConfig *ConfigAdapter::searchLocationConfig(
    const ServerConfig &server_conf, const std::string &path) {
  return NULL;
  (void)server_conf;
  (void)path;
}

std::string ConfigAdapter::makeAbsolutePath(const LocationConfig &location_conf,
                                            const std::string &path) {
  return "/var/www/html/" + path;
  (void)location_conf;
}

const std::string *ConfigAdapter::searchRedirectUri(
    const LocationConfig &location_conf) {
  return NULL;
  (void)location_conf;
}

int ConfigAdapter::searchRedirectStatusCode(
    const LocationConfig &location_conf) {
  return 301;
  (void)location_conf;
}

bool isCgiPath(const LocationConfig &location_conf, const std::string &path) {
  return false;
  (void)location_conf;
  (void)path;
}

const std::string *ConfigAdapter::searchErrorPage(
    const ServerConfig &server_conf, int code) {
  return NULL;
  (void)server_conf;
  (void)code;
}

bool ConfigAdapter::isAllowMethods(const LocationConfig &location_conf,
                                   const std::string &method) {
  return true;
  (void)location_conf;
  (void)method;
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const LocationConfig &location_conf) {
  std::vector<std::string> methods;

  methods.push_back("GET");
  methods.push_back("DELETE");
  methods.push_back("POST");
  return methods;
  (void)location_conf;
}

const std::string *ConfigAdapter::searchHostName(
    const ServerConfig &server_conf) {
  return NULL;
  (void)server_conf;
}

bool ConfigAdapter::isAutoindex(const LocationConfig &location_conf) {
  return false;
  (void)location_conf;
}

std::string ConfigAdapter::searchIndex(const LocationConfig &location_conf) {
  return "/var/www/html/index.html";
  (void)location_conf;
}

size_t ConfigAdapter::getClientMaxBodySize(
    const LocationConfig &location_conf) {
  return INTERNAL::DEFAULT_MAX_CLIENT_BODY_SIZE;
  (void)location_conf;
}

size_t ConfigAdapter::getMaxNumberOfCrlfBeforeMethod(void) {
  return INTERNAL::DEFAULT_MAX_NUMBER_OF_CRLF_BEFORE_METHOD;
}

size_t ConfigAdapter::getMaxMethodSize(void) {
  return INTERNAL::DEFAULT_MAX_METHOD_SIZE;
}

size_t ConfigAdapter::getMaxUriSize(void) {
  return INTERNAL::DEFAULT_MAX_URI_SIZE;
}

size_t ConfigAdapter::getMaxHeaderSize(void) {
  return INTERNAL::DEFAULT_MAX_HEADER_SIZE;
}

size_t ConfigAdapter::getMaxNumberOfHeaders(void) {
  return INTERNAL::DEFAULT_MAX_NUMBER_OF_HEADERS;
}

size_t ConfigAdapter::getMaxBodySize(void) {
  return INTERNAL::DEFAULT_MAX_BODY_SIZE;
}

bool ConfigAdapter::isCorrespondingMethod(const std::string &method) {
  return (method == "GET" || method == "POST" || method == "DELETE");
}
