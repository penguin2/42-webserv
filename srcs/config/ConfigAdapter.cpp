#include "config/ConfigAdapter.hpp"

#include <algorithm>

#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

const ServerConfig &ConfigAdapter::searchServerConfig(const std::string &host,
                                                      const std::string &port) {
  const std::vector<ServerConfig> &server_confs =
      Config::getInstance().getServers();

  for (std::vector<ServerConfig>::const_iterator it = server_confs.begin();
       it != server_confs.end(); ++it) {
    if (it->getServerName() == host && it->getListenAddress() == port)
      return *it;
  }
  return *server_confs.begin();
}

const LocationConfig *ConfigAdapter::searchLocationConfig(
    const ServerConfig &server_conf, const std::string &path) {
  const std::map<std::string, LocationConfig> &location_confs =
      server_conf.getLocationConfigs();

  for (std::map<std::string, LocationConfig>::const_iterator it =
           location_confs.begin();
       it != location_confs.end(); ++it) {
    if (path == it->first) return &it->second;
  }
  return NULL;
}

std::string ConfigAdapter::makeAbsolutePath(const LocationConfig &location_conf,
                                            const std::string &path) {
  static const std::string DEFAULT_ROOT = "/var/www/html";
  const std::string &root = location_conf.getRoot();

  if (root.empty()) return DEFAULT_ROOT + path;
  return root + path;
}

const std::string *ConfigAdapter::searchRedirectUri(
    const LocationConfig &location_conf) {
  const std::string &uri = location_conf.getReturnUri();

  if (uri.empty()) return NULL;
  return &uri;
}

int ConfigAdapter::searchRedirectStatusCode(
    const LocationConfig &location_conf) {
  return location_conf.getReturnStatusCode();
}

bool isCgiPath(const LocationConfig &location_conf, const std::string &path) {
  return false;
  (void)location_conf;
  (void)path;
}

const std::string *ConfigAdapter::searchErrorPage(
    const ServerConfig &server_conf, int code) {
  const std::string &error_page = server_conf.getErrorPage(code);

  if (error_page.empty()) return NULL;
  return &error_page;
}

bool ConfigAdapter::isAllowMethods(const LocationConfig &location_conf,
                                   const std::string &method) {
  const std::vector<HttpMethod> &allow_methods =
      location_conf.getAllowMethods();
  HttpMethod http_method;

  if (method == "GET") http_method = GET;
  if (method == "POST") http_method = POST;
  if (method == "DELETE") http_method = DELETE;
  return (std::find(allow_methods.begin(), allow_methods.end(), http_method) !=
          allow_methods.end());
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const LocationConfig &location_conf) {
  std::vector<std::string> methods;

  if (isAllowMethods(location_conf, "GET")) methods.push_back("GET");
  if (isAllowMethods(location_conf, "DELETE")) methods.push_back("DELETE");
  if (isAllowMethods(location_conf, "POST")) methods.push_back("POST");
  return methods;
}

const std::string *ConfigAdapter::searchHostName(
    const ServerConfig &server_conf) {
  const std::string &host_name = server_conf.getServerName();

  if (host_name.empty()) return NULL;
  return &host_name;
}

bool ConfigAdapter::isAutoindex(const LocationConfig &location_conf) {
  return location_conf.getAutoindex();
}

std::string ConfigAdapter::searchIndex(const LocationConfig &location_conf) {
  return location_conf.getIndex();
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
