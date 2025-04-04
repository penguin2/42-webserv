#include "config/ConfigAdapter.hpp"

#include <algorithm>
#include <cstdlib>

#include "CgiRequest.hpp"
#include "ListenSocket.hpp"
#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SysUtils.hpp"
#include "utils/Utils.hpp"

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

    const int listen_socket_fd = sys_utils::makeListenSocket(
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
    const std::string& http_path,
    const std::map<std::string, LocationConfig>& location_configs) {
  for (std::map<std::string, LocationConfig>::const_reverse_iterator it =
           location_configs.rbegin();
       it != location_configs.rend(); ++it) {
    const std::string& location_path = it->first;
    const LocationConfig* location_config = &(it->second);
#if defined(__MACH__)
    if (utils::isStartsWithCaseInsensitive(http_path, location_path))
      return location_config;
#elif defined(__linux__)
    if (utils::isStartsWith(http_path, location_path)) return location_config;
#endif
  }
  return NULL;
}

std::string ConfigAdapter::makeFilePath(const LocationConfig& location_conf,
                                        const std::string& http_path) {
  const std::string& root = location_conf.getRoot();

  if (root.empty()) return INTERNAL::DEFAULT_ROOT + http_path;
  if (root == "/") return http_path;
  return utils::concatWithSlash(root, http_path);
}

const std::string* ConfigAdapter::searchRedirectUri(
    const LocationConfig& location_conf) {
  const std::string& redirect_uri = location_conf.getReturnUri();

  if (redirect_uri.empty()) return NULL;
  return &redirect_uri;
}

size_t ConfigAdapter::searchRedirectStatusCode(
    const LocationConfig& location_conf) {
  return location_conf.getReturnStatusCode();
}

bool ConfigAdapter::isCgiPath(const LocationConfig& location_conf,
                              const std::string& http_path) {
  std::string file_path = makeFilePath(location_conf, http_path);
  std::map<std::string, std::string> file_data_map =
      makeFileDataMap(location_conf, file_path);
  std::string path_to_cgi_script = file_data_map["DIR"] + file_data_map["FILE"];

  if (file_data_map["FILE"].empty()) return false;
  return file_utils::isExistFile(path_to_cgi_script);
}

const std::string* ConfigAdapter::searchErrorPage(
    const ServerConfig& server_conf, size_t status_code) {
  const std::string& error_page = server_conf.getErrorPage(status_code);

  if (error_page.empty()) return NULL;
  return &error_page;
}

bool ConfigAdapter::isAllowMethods(const LocationConfig& location_conf,
                                   const std::string& method) {
  const std::vector<std::string>& methods = location_conf.getAllowMethods();

  if (methods.empty()) return true;
  return (std::find(methods.begin(), methods.end(), method) != methods.end());
}

std::vector<std::string> ConfigAdapter::getAllowMethods(
    const LocationConfig& location_conf) {
  static const std::vector<std::string> corresponding_methods =
      Config::makeCorrespondingMethods();
  const std::vector<std::string>& allow_methods =
      location_conf.getAllowMethods();

  if (allow_methods.empty()) return corresponding_methods;
  return allow_methods;
}

const std::string* ConfigAdapter::searchHostName(
    const ServerConfig& server_conf) {
  const std::string& host_name = server_conf.getServerName();

  if (host_name.empty()) return NULL;
  return &host_name;
}

const std::string& ConfigAdapter::getListenPort(
    const ServerConfig& server_conf) {
  return server_conf.getListenPort();
}

bool ConfigAdapter::isAutoindex(const LocationConfig& location_conf) {
  return location_conf.getAutoindex();
}

std::string ConfigAdapter::searchIndex(const LocationConfig& location_conf) {
  return location_conf.getIndex();
}

std::vector<std::string> ConfigAdapter::getCgiExts(
    const LocationConfig& location_conf) {
  return location_conf.getCgiExt();
}

std::map<std::string, std::string> ConfigAdapter::makeFileDataMap(
    const LocationConfig& location_conf, const std::string& file_path) {
  const std::vector<std::string>& exts = getCgiExts(location_conf);
  std::map<std::string, std::string> file_data_map;

  for (std::vector<std::string>::const_iterator it_ext = exts.begin();
       it_ext != exts.end(); ++it_ext) {
    file_data_map = makeFileDataMapFromFilePath(file_path, *it_ext);
    if (!file_data_map["FILE"].empty()) return file_data_map;
  }
  return file_data_map;
}

std::map<std::string, std::string> ConfigAdapter::makeFileDataMapFromFilePath(
    const std::string& file_path, const std::string& ext) {
  std::vector<std::string> path_elements = utils::split(file_path, '/');
  std::map<std::string, std::string> file_data_map;
  file_data_map["DIR"] = utils::isStartsWith(file_path, "/") ? "/" : "";
  file_data_map["FILE"] = "";
  file_data_map["PATH_INFO"] = "";

  std::vector<std::string>::const_iterator it = path_elements.begin();
  for (; it != path_elements.end(); ++it) {
    if (utils::isEndsWith(*it, ext)) {
      file_data_map["FILE"] = *it;
      ++it;
      break;
    }
    file_data_map["DIR"].append(*it).append("/");
  }
  if (file_data_map["FILE"].empty()) return file_data_map;
  for (; it != path_elements.end(); ++it) {
    file_data_map["PATH_INFO"].append("/").append(*it);
  }
  if (utils::isEndsWith(file_path, "/")) file_data_map["PATH_INFO"].append("/");
  return file_data_map;
}

bool ConfigAdapter::canUpload(const LocationConfig& location_conf) {
  return location_conf.getAllowUpload();
}

size_t ConfigAdapter::getClientMaxBodySize(
    const LocationConfig& location_conf) {
  return location_conf.getClientMaxBodySize();
}

size_t ConfigAdapter::getMaxNumberOfCrlfBeforeMethod(void) {
  return INTERNAL::DEFAULT_MAX_NUMBER_OF_CRLF_BEFORE_METHOD;
}

size_t ConfigAdapter::getMaxMethodSize(void) {
  static const std::string longest_method =
      utils::findLongestString(Config::makeCorrespondingMethods());
  return longest_method.size();
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

bool ConfigAdapter::isCorrespondingMethod(const std::string& method) {
  return Config::isCorrespondingMethod(method);
}

size_t ConfigAdapter::getMaxLocalRedirectCount(void) {
  return INTERNAL::DEFAULT_MAX_LOCAL_REDIRECT_COUNT;
}
