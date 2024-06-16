#ifndef WEBSERV_URIUTILS_H_
#define WEBSERV_URIUTILS_H_

#include <string>

namespace uri_utils {
int isUnreserved(int c);
int isSubDelims(int c);
int isRegName(int c);
int isRegNameWithoutPctEncoding(int c);
int isPctEncodingCharset(int c);
int isUserInfoCharset(int c);
int isPathCharset(int c);
int isPathCharsetWithoutPctEncoding(int c);
int isQueryCharset(int c);

bool decodeUrlEncoding(std::string& str);
bool isIPv4Address(const std::string& str);
std::string removeDotSegments(const std::string& path);
}  // namespace uri_utils

#endif
