#include "config/ConfigAdapter.hpp"

#include <cstdlib>

#include "ListenSocket.hpp"
#include "SysUtils.hpp"
#include "Utils.hpp"
#include "config/Config.hpp"

std::map<SocketAddress, std::vector<const ServerConfig*> >
ConfigAdapter::makeServerConfigGroups() {
  std::map<SocketAddress, std::vector<const ServerConfig*> >
      server_config_groups;
  const std::vector<ServerConfig>& server_configs =
      Config::getInstance().getServers();

  for (std::vector<ServerConfig>::const_iterator it = server_configs.begin();
       it != server_configs.end(); ++it) {
    const std::string& ip_addr = it->getListenAddress();
    const std::string& port = it->getListenPort();
    const SocketAddress socket_address = SocketAddress(ip_addr, port);

    server_config_groups[socket_address].push_back(&(*it));
  }

  return server_config_groups;
}

std::map<int, ListenSocket*> ConfigAdapter::makeInitialListenSockets() {
  std::map<int, ListenSocket*> initial_listen_sockets;
  const std::map<SocketAddress, std::vector<const ServerConfig*> >&
      server_config_groups = ConfigAdapter::makeServerConfigGroups();

  for (std::map<SocketAddress,
                std::vector<const ServerConfig*> >::const_iterator it =
           server_config_groups.begin();
       it != server_config_groups.end(); ++it) {
    const SocketAddress& socket_address = it->first;
    const std::vector<const ServerConfig*>& server_configs = it->second;

    const int listen_socket_fd = SysUtils::makeListenSocket(
        socket_address.getIpAddr().c_str(), socket_address.getPort().c_str(),
        ConfigAdapter::INTERNAL::DEFAULT_LISTEN_BACKLOG);
    if (listen_socket_fd < 0) continue;

    ListenSocket* new_listen_socket =
        new ListenSocket(listen_socket_fd, socket_address);
    new_listen_socket->setServerConfigs(server_configs);

    initial_listen_sockets[listen_socket_fd] = new_listen_socket;
  }

  return initial_listen_sockets;
}

const ServerConfig* ConfigAdapter::searchServerConfig(
    const std::string& host,
    const std::vector<const ServerConfig*>& server_configs) {
  const ServerConfig* default_server_config = *server_configs.begin();

  for (std::vector<const ServerConfig*>::const_iterator it =
           server_configs.begin();
       it != server_configs.end(); ++it)
    if (host == (*it)->getServerName()) return *it;

  return default_server_config;
}

// taking advantage of 'sorted keys' for std::map<std::string, T>,
// checking keys in reverse order assures
// that the first prefix found is the longest prefix matching
const LocationConfig* ConfigAdapter::searchLocationConfig(
    const std::string& path,
    const std::map<std::string, LocationConfig>& location_configs) {
  for (std::map<std::string, LocationConfig>::const_reverse_iterator it =
           location_configs.rbegin();
       it != location_configs.rend(); ++it) {
    const std::string& location_path = it->first;
    const LocationConfig* location_config = &(it->second);
#if defined(__MACH__)
    if (Utils::isStartsWithCaseInsensitive(path, location_path))
      return location_config;
#elif defined(__linux__)
    if (Utils::isStartsWith(path, location_path)) return location_config;
#endif
  }
  return NULL;
}

const std::string* ConfigAdapter::searchRedirectUri(const std::string& host,
                                                    size_t port,
                                                    const std::string& path) {
  static const std::string mock_redirect_uri(
      "/var/www/html/new-location/index.html");

  if (rand() % 2 == 0) return &mock_redirect_uri;
  return NULL;
  (void)host;
  (void)port;
  (void)path;
}

int ConfigAdapter::searchRedirectStatusCode(const std::string& host,
                                            size_t port,
                                            const std::string& path) {
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

bool ConfigAdapter::isCgiPath(const std::string& host, size_t port,
                              const std::string& path) {
  return (rand() % 2 == 0);
  (void)host;
  (void)port;
  (void)path;
}

const std::string* ConfigAdapter::searchErrorPage(const std::string& host,
                                                  size_t port,
                                                  size_t status_code) {
  static const std::string mock_error_page("/var/www/html/error/error.html");

  if (rand() % 2 == 0) return &mock_error_page;
  return NULL;
  (void)host;
  (void)port;
  (void)status_code;
}

bool ConfigAdapter::isAllowMethods(const std::string& host, size_t port,
                                   const std::string& path,
                                   const std::string& method) {
  return (rand() % 2 == 0);
  (void)host;
  (void)port;
  (void)path;
  (void)method;
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const std::string& host, size_t port, const std::string& path) {
  std::vector<std::string> allow_methods;

  if (rand() % 2 == 0) allow_methods.push_back("GET");
  if (rand() % 2 == 0) allow_methods.push_back("POST");
  if (rand() % 2 == 0) allow_methods.push_back("DELETE");
  return allow_methods;
  (void)host;
  (void)port;
  (void)path;
}

bool ConfigAdapter::isCorrespondingMethod(const std::string& method) {
  return (method == "GET" || method == "POST" || method == "DELETE");
}

std::vector<std::string> ConfigAdapter::makeAbsolutePaths(
    const std::string& host, size_t port, const std::string& path) {
  const std::string base_dir("/var/www/html");
  const std::string base_cgi_dir("/lib");
  std::vector<std::string> absolute_paths;

  if (path.compare(0, 9, "/cgi-bin/") == 0) {
    absolute_paths.push_back(base_cgi_dir + path);
  } else if (path == "" || path == "/") {
    absolute_paths.push_back(base_dir + "/index.html");
    absolute_paths.push_back(base_dir + "/app/index.html");
    absolute_paths.push_back(base_dir + "/form/index.html");
  } else {
    absolute_paths.push_back(base_dir + path);
    absolute_paths.push_back(base_dir + "/app" + path);
    absolute_paths.push_back(base_dir + "/form" + path);
  }
  return absolute_paths;
  (void)host;
  (void)port;
}

size_t ConfigAdapter::getMaxBodySize(const std::string& host, size_t port,
                                     const std::string& path) {
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

// #include <iostream>
// using namespace ConfigAdapter;
// using namespace std;
// int main(void) {
//   srand(time(NULL));
//   const std::string* uri = searchRedirectUri("", 0, "");
//   cout << (uri ? *uri : "NULL") << endl;
//   cout << searchRedirectStatusCode("", 0, "") << endl;
//   cout << isCgiPath("", 0, "") << endl;
//   const std::string *page = searchErrorPage("", 0, 400);
//   cout << (page ? *page : "NULL") << endl;
//   cout << isAllowMethods("", 0, "", "") << endl;
//   vector<string> methods = getAllowMethods("", 0, "");
//   cout << (1 <= methods.size() ? methods[0] : "")
//        << (2 <= methods.size() ? ", " + methods[1] : "")
//        << (3 <= methods.size() ? ", " + methods[2] : "");
//   cout << endl;
//   vector<string> paths = makeAbsolutePaths("", 0, "/");
//   for (vector<string>::const_iterator it = paths.begin(); it != paths.end();
//        ++it)
//     cout << *it << " ";
//   cout << endl;
//   vector<string> cgi_paths = makeAbsolutePaths("", 0, "/cgi-bin/cgi.py");
//   for (vector<string>::const_iterator it = cgi_paths.begin();
//        it != cgi_paths.end(); ++it)
//     cout << *it << " ";
//   cout << endl;
//   cout << getMaxHeaderSize() << endl;
//   cout << getMaxUriSize() << endl;
//   cout << getMaxBodySize("", 0, "") << endl;
//   return 0;
// }
