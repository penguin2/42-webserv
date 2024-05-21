#ifndef WEBSERV_URIUTILS_H_
#define WEBSERV_URIUTILS_H_

#include <string>

class UriUtils {
 public:
  static int isUnreserved(int c);
  static int isSubDelims(int c);
  static int isRegName(int c);
  static int isPctEncodingCharset(int c);
  static int isUserInfoCharset(int c);
  static int isPathCharset(int c);
  static int isPathCharsetWithoutPctEncoding(int c);
  static int isQueryCharset(int c);

  static bool decodeUrlEncoding(std::string& str);
  static bool isIPv4Address(const std::string& str);
  static std::string removeDotSegments(const std::string& path);

 private:
  UriUtils(void);
  ~UriUtils(void);
  UriUtils(const UriUtils&);
  void operator=(const UriUtils&);
};

#endif
