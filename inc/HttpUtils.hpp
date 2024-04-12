#ifndef WEBSERV_HTTPUTILS_H_
#define WEBSERV_HTTPUTILS_H_

#include <string>

class HttpUtils {
 public:
  static std::string generatePage(const std::string& file);
  static std::string generateErrorPage(int code, const std::string& phrase);
  static std::string generateErrorPage(const std::string& file, int code,
                                       const std::string& phrase);
  static std::string generateDateValue(void);
  static std::string generateContentType(const std::string& path);
  static bool isMaintainConnection(int code);

 private:
  HttpUtils(void);
  ~HttpUtils(void);
  HttpUtils(const HttpUtils&);
  void operator=(const HttpUtils&);
};

#endif
