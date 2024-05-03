#include "./config/ConfigAdapter.hpp"

#include <cstdlib>

const std::string *ConfigAdapter::searchRedirectUri(const std::string &host,
                                                    size_t port,
                                                    const std::string &path) {
  static const std::string mock_redirect_uri(
      "/var/www/html/new-location/index.html");

  return NULL;
  (void)host;
  (void)port;
  (void)path;
}

int ConfigAdapter::searchRedirectStatusCode(const std::string &host,
                                            size_t port,
                                            const std::string &path) {
  return 301;
  (void)host;
  (void)port;
  (void)path;
}

bool ConfigAdapter::isCgiPath(const std::string &host, size_t port,
                              const std::string &path) {
  return false;
  (void)host;
  (void)port;
  (void)path;
}

const std::string *ConfigAdapter::searchErrorPage(const std::string &host,
                                                  size_t port,
                                                  size_t status_code) {
  static const std::string mock_error_page("/var/www/html/error/error.html");

  return &mock_error_page;
  (void)host;
  (void)port;
  (void)status_code;
}

bool ConfigAdapter::isAllowMethods(const std::string &host, size_t port,
                                   const std::string &path,
                                   const std::string &method) {
  return true;
  (void)host;
  (void)port;
  (void)path;
  (void)method;
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const std::string &host, size_t port, const std::string &path) {
  std::vector<std::string> allow_methods;

  allow_methods.push_back("GET");
  allow_methods.push_back("POST");
  allow_methods.push_back("DELETE");
  return allow_methods;
  (void)host;
  (void)port;
  (void)path;
}

bool ConfigAdapter::isCorrespondingMethod(const std::string &method) {
  return (method == "GET" || method == "POST" || method == "DELETE");
}

std::vector<std::string> ConfigAdapter::makeAbsolutePaths(
    const std::string &host, size_t port, const std::string &path) {
  const std::string base_dir("/var/www/html");
  std::vector<std::string> absolute_paths;

  if (host == "DELETE") {
    absolute_paths.push_back(base_dir + path + "/");
    absolute_paths.push_back(base_dir + path);

  } else if (host == "POST") {
    absolute_paths.push_back(base_dir + path);
    absolute_paths.push_back(base_dir + path + "/");
  } else {
    absolute_paths.push_back(base_dir + path);
    absolute_paths.push_back(base_dir + path + "/");
    absolute_paths.push_back(base_dir + path + "/index.html");
    absolute_paths.push_back(base_dir + "/index.html");
  }
  return absolute_paths;
  (void)host;
  (void)port;
}

bool ConfigAdapter::isAutoindexOn(const std::string &host, size_t port,
                                  const std::string &path) {
  return true;
  (void)host;
  (void)port;
  (void)path;
}

size_t ConfigAdapter::getMaxBodySize(const std::string &host, size_t port,
                                     const std::string &path) {
  return INTERNAL::DEFAULT_MAX_BODY_SIZE;
  (void)host;
  (void)port;
  (void)path;
}

size_t ConfigAdapter::getMaxHeaderSize(void) {
  return INTERNAL::DEFAULT_MAX_HEADER_SIZE;
}

size_t ConfigAdapter::getMaxNumberOfHeaders(void) {
  return INTERNAL::DEFAULT_MAX_NUMBER_OF_HEADERS;
}

size_t ConfigAdapter::getMaxUriSize(void) {
  return INTERNAL::DEFAULT_MAX_URI_SIZE;
}

size_t ConfigAdapter::getMaxMethodSize(void) {
  return INTERNAL::DEFAULT_MAX_METHOD_SIZE;
}

size_t ConfigAdapter::getMaxNumberOfCrlfBeforeMethod(void) {
  return INTERNAL::DEFAULT_MAX_NUMBER_OF_CRLF_BEFORE_METHOD;
}
