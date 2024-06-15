#ifndef WEBSERV_UTILS_H_
#define WEBSERV_UTILS_H_

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace utils {
void strTrim(std::string& str, const char* charset);
void strLTrim(std::string& str, const char* charset);
bool isContain(const std::string& str, int (*is_func)(int));
bool isContain(const std::string& str, const std::string charset);
bool isContainsOnly(const std::string& str, int (*is_func)(int));
bool isContainsOnly(const std::string& str, const std::string charset);
void toLowerString(std::string& str);
std::string toLower(const std::string& str);
void toUpperString(std::string& str);
std::string toUpper(const std::string& str);
bool strToSize_t(const std::string& str, size_t& num, int base);
bool isStartsWith(const std::string& str, const std::string& prefix);
bool isEndsWith(const std::string& str, const std::string& suffix);
bool isStartsWithCaseInsensitive(const std::string& str,
                                 const std::string& prefix);
std::string popFrontSubstr(std::string& str, std::size_t n);
std::string getExtension(const std::string& file);
std::string findLongestString(const std::vector<std::string>& strings);

template <typename T>
bool parseValue(const std::string& str, T& value) {
  std::istringstream ss(str);
  return !(ss >> value).fail() && ss.eof();
}

std::vector<std::string> split(const std::string& str, char separator);
std::string joinStrings(const std::vector<std::string>& strings,
                        std::string delimiter);
bool isSameValueCaseInsensitive(const std::map<std::string, std::string>& mp,
                                const std::string& key,
                                const std::string& value);
std::string uintToString(unsigned int val);
bool hasContentInMap(const std::map<std::string, std::string>& mp,
                     const std::string& key);
std::string concatWithSlash(const std::string& s1, const std::string& s2);
}  // namespace utils

#endif
