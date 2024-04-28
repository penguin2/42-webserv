#ifndef WEBSERV_HTTPUTILS_H_
#define WEBSERV_HTTPUTILS_H_

#include <map>
#include <set>
#include <string>

class HttpUtils {
 public:
  static bool generateAutoindexPage(const std::string& dir,
                                    std::stringstream& ss);
  static bool generateDirectoryIndex(struct dirent, const std::string& dir,
                                     std::stringstream& ss);
  static std::string generateErrorPage(int code, const std::string& phrase);
  static std::string generateErrorPage(const std::string* file, int code,
                                       const std::string& phrase);
  static std::string generateDateValue(void);
  static std::string convertPathToContentType(const std::string& path);
  static bool isMaintainConnection(int code);
  static bool isRedirectStatusCode(int code);
  static std::map<std::string, std::string> makeContentTypeMap(void);
  static std::set<int> makeDisconnectCodeSet(void);
  static std::set<int> makeRedirectCodeSet(void);

 private:
  HttpUtils(void);
  ~HttpUtils(void);
  HttpUtils(const HttpUtils&);
  void operator=(const HttpUtils&);
};

#endif
