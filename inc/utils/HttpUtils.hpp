#ifndef WEBSERV_HTTPUTILS_H_
#define WEBSERV_HTTPUTILS_H_

#include <dirent.h>

#include <ctime>
#include <map>
#include <set>
#include <string>

#include "utils/FileUtils.hpp"

namespace http_utils {
std::string generateErrorPage(size_t status_code, const std::string& phrase);
std::string generateErrorPage(const std::string* file, size_t status_code,
                              const std::string& phrase);
std::string generateDateAsFormat(std::time_t t, const std::string& fmt);
std::string convertPathToContentType(const std::string& path);
bool isKeepConnection(size_t code);
bool isRedirectStatusCode(size_t code);
std::map<std::string, std::string> makeContentTypeMap(void);
std::set<size_t> makeKeepConnectionCodeSet(void);
std::set<size_t> makeRedirectCodeSet(void);
bool generateAutoindexPage(const std::string& path_component,
                           const std::string& dir_path, std::stringstream& ss);
bool isStatusCode(size_t code);
int isHeaderKeyChar(int c);
int isCookieValueChar(int c);
bool isFullDateRFC1123(const std::string& date_str);
std::string generateRedirectContent(const std::string& uri, size_t status_code,
                                    const std::string& phrase);

namespace autoindex_utils {
bool generateFileRecord(const file_utils::Entry& entry, const std::string& dir,
                        std::stringstream& ss);
void generateFileLink(const std::string& file_name, std::stringstream& ss);
bool generateFileDetail(const std::string& file_path, bool is_dir,
                        std::stringstream& ss);
bool generateFileDetailTimestamp(const std::string& file_path,
                                 std::stringstream& ss);

}  // namespace autoindex_utils

namespace is_full_date_utils {
bool isWkDay(const std::string& wkday_str);
std::vector<std::string> makeWkDay(void);
bool isDate1(const std::string& date1_str);
bool isMonth(const std::string& month_str);
std::vector<std::string> makeMonth(void);
bool isTime(const std::string& time_str);
}  // namespace is_full_date_utils
}  // namespace http_utils

#endif
