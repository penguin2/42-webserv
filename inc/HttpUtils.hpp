#ifndef WEBSERV_HTTPUTILS_H_
#define WEBSERV_HTTPUTILS_H_

#include <dirent.h>

#include <ctime>
#include <map>
#include <set>
#include <string>

#include "FileUtils.hpp"

namespace HttpUtils {
std::string generateErrorPage(int code, const std::string& phrase);
std::string generateErrorPage(const std::string* file, int code,
                              const std::string& phrase);
std::string generateDateAsFormat(std::time_t t, const std::string& fmt);
std::string convertPathToContentType(const std::string& path);
bool isMaintainConnection(int code);
bool isRedirectStatusCode(size_t code);
std::map<std::string, std::string> makeContentTypeMap(void);
std::set<int> makeDisconnectCodeSet(void);
std::set<size_t> makeRedirectCodeSet(void);
bool generateAutoindexPage(const std::string& path_component,
                           const std::string& absolute_dir_path,
                           std::stringstream& ss);
bool isStatusCode(size_t code);

namespace AutoindexUtils {
bool generateFileRecord(const FileUtils::Entry& entry, const std::string& dir,
                        std::stringstream& ss);
void generateFileLink(const std::string& file_name, std::stringstream& ss);
bool generateFileDetail(const std::string& file_path, bool is_dir,
                        std::stringstream& ss);
bool generateFileDetailTimestamp(const std::string& path,
                                 std::stringstream& ss);

}  // namespace AutoindexUtils
}  // namespace HttpUtils

#endif
