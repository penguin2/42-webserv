#ifndef WEBSERV_CONFIGADAPTER_H_
#define WEBSERV_CONFIGADAPTER_H_

#include <map>
#include <string>
#include <vector>

#include "ListenSocket.hpp"
#include "LocationConfig.hpp"
#include "SocketAddress.hpp"
#include "config/ServerConfig.hpp"

namespace ConfigAdapter {

std::map<SocketAddress, std::vector<const ServerConfig*> >
makeServerConfigGroups();

std::map<int, ListenSocket*> makeInitialListenSockets();

const ServerConfig* searchServerConfig(
    const std::string& host,
    const std::vector<const ServerConfig*>& server_configs);

const LocationConfig* searchLocationConfig(
    const std::string& http_path,
    const std::map<std::string, LocationConfig>& location_configs);

std::string makeFilePath(const LocationConfig& location_conf,
                         const std::string& http_path);

// (リダイレクト対象のRequest) ? redirect_URI : NULL
const std::string* searchRedirectUri(const LocationConfig& location_conf);

// (リダイレクト対象のRequest) ? redirect_status_code : 0
// searchRedirectUri関数後に実行される想定のため、0が返ることは基本ない
size_t searchRedirectStatusCode(const LocationConfig& location_conf);

// (Locationにcgi_path&cgi_extがあり、PathがCGIファイル) ? true : false
bool isCgiPath(const LocationConfig& location_conf, const std::string& path);

// (error_pageに該当するStatusCodeが含まれる) ? error_page_path : NULL
const std::string* searchErrorPage(const ServerConfig& server_conf,
                                   size_t status_code);

// allow_methodsディレクティブが存在しない場合、全てのMethodが許容される
bool isAllowMethods(const LocationConfig& location_conf,
                    const std::string& method);

std::vector<std::string> getAllowMethods(const LocationConfig& location_conf);

const std::string* searchHostName(const ServerConfig& server_conf);

const std::string& getListenPort(const ServerConfig& server_conf);

bool isAutoindex(const LocationConfig& location_conf);

std::string searchIndex(const LocationConfig& location_conf);

std::vector<std::string> getCgiExts(const LocationConfig& location_conf);

// FileDataMap contains the elements DIR, FILE, and PATH_INFO
// if (FileDataMap["FILE"] == "") -> Not CGI Pattern
std::map<std::string, std::string> makeFileDataMap(
    const LocationConfig& location_conf, const std::string& file_path);
std::map<std::string, std::string> makeFileDataMapFromFilePath(
    const std::string& file_path, const std::string& ext);

size_t getClientMaxBodySize(const LocationConfig& location_conf);

size_t getMaxNumberOfCrlfBeforeMethod(void);
size_t getMaxMethodSize(void);
size_t getMaxUriSize(void);
size_t getMaxHeaderSize(void);
size_t getMaxNumberOfHeaders(void);
size_t getMaxBodySize(void);

bool isCorrespondingMethod(const std::string& method);

namespace INTERNAL {
const int DEFAULT_LISTEN_BACKLOG = 511;
const size_t DEFAULT_MAX_NUMBER_OF_CRLF_BEFORE_METHOD = 10;
const size_t DEFAULT_MAX_URI_SIZE = 2000;
const size_t DEFAULT_MAX_HEADER_SIZE = 200;
const size_t DEFAULT_MAX_NUMBER_OF_HEADERS = 100;
const size_t DEFAULT_MAX_BODY_SIZE = 1000000000;
const size_t DEFAULT_MAX_CLIENT_BODY_SIZE = 0;
const std::string DEFAULT_ROOT = "/var/www/html";
}  // namespace INTERNAL

}  // namespace ConfigAdapter

#endif
