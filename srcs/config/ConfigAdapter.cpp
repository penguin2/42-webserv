#include "ConfigAdapter.hpp"

#include <cstdlib>

const std::string *ConfigAdapter::searchRedirectUri(const std::string &host,
                                                    size_t port,
                                                    const std::string &path) {
  static const std::string mock_redirect_uri("/var/www/new-location");

  if (rand() % 2 == 0) return &mock_redirect_uri;
  return NULL;
  (void)host;
  (void)port;
  (void)path;
}

int ConfigAdapter::searchRedirectStatusCode(const std::string &host,
                                            size_t port,
                                            const std::string &path) {
  int random_number = (rand() % 6);
  int status_code;
  switch (random_number) {
    case 0:
      status_code = 301;
      break;
    case 1:
      status_code = 302;
      break;
    case 2:
      status_code = 303;
      break;
    case 3:
      status_code = 307;
      break;
    case 4:
      status_code = 308;
      break;
    case 5:
      status_code = 0;
      break;
    default:
      status_code = 0;
      break;
  }
  return status_code;
  (void)host;
  (void)port;
  (void)path;
}

bool ConfigAdapter::isCgiPath(const std::string &host, size_t port,
                              const std::string &path) {
  return (rand() % 2 == 0);
  (void)host;
  (void)port;
  (void)path;
}

const std::string *ConfigAdapter::searchErrorPage(const std::string &host,
                                                  size_t port,
                                                  size_t status_code) {
  static const std::string mock_error_page("/var/www/error/4xx.html");

  if (rand() % 2 == 0) return &mock_error_page;
  return NULL;
  (void)host;
  (void)port;
  (void)status_code;
}

bool ConfigAdapter::isAllowMethods(const std::string &host, size_t port,
                                   const std::string &path,
                                   const std::string &method) {
  return (rand() % 2 == 0);
  (void)host;
  (void)port;
  (void)path;
  (void)method;
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const std::string &host, size_t port, const std::string &path) {
  std::vector<std::string> allow_methods;

  if (rand() % 2 == 0) allow_methods.push_back("GET");
  if (rand() % 2 == 0) allow_methods.push_back("POST");
  if (rand() % 2 == 0) allow_methods.push_back("DELETE");
  return allow_methods;
  (void)host;
  (void)port;
  (void)path;
}

std::string ConfigAdapter::makeAbsolutePath(const std::string &host,
                                            size_t port,
                                            const std::string &path) {
  const std::string base_dir("/var/www/html");
  const std::string base_cgi_dir("/lib");

  if (path.compare(0, 9, "/cgi-bin/") == 0) return base_cgi_dir + path;
  if (path == "" || path == "/") return base_dir + "/index.html";
  return base_dir + path;
  (void)host;
  (void)port;
}

size_t ConfigAdapter::getMaxBodySize(const std::string &host, size_t port,
                                     const std::string &path) {
  return 10000;
  (void)host;
  (void)port;
  (void)path;
}

size_t ConfigAdapter::getMaxHeaderSize(void) { return 200; }

size_t ConfigAdapter::getMaxUriSize(void) { return 2000; }

// #include <iostream>
// using namespace ConfigAdapter;
// using namespace std;
// int main(void) {
//   srand(time(NULL));
//   const std::string *uri = searchRedirectUri("", 0, "");
//   cout << (uri ? *uri : "NULL") << endl;
//   cout << searchRedirectStatusCode("", 0, "") << endl;
//   cout << isCgiPath("", 0, "") << endl;
//   const std::string *page = searchErrorPage("", 0, 400);
//   cout << (page ? *page : "NULL") << endl;
//   cout << isAllowMethods("", 0, "", "") << endl;
//   vector<string> methods = getAllowMethods("", 0, "");
//   cout << (1 <= methods.size() ? methods[0] : "")
//        << (2 <= methods.size() ? ", " + methods[1] : "")
//        << (2 <= methods.size() ? ", " + methods[2] : "");
//   cout << endl;
//   cout << makeAbsolutePath("", 0, "/") << endl;
//   cout << makeAbsolutePath("", 0, "/cgi-bin/cgi.py") << endl;
//   cout << getMaxHeaderSize() << endl;
//   cout << getMaxUriSize() << endl;
//   cout << getMaxBodySize("", 0, "") << endl;
//   return 0;
// }
