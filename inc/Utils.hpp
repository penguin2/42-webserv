#ifndef WEBSERV_UTILS_H_
#define WEBSERV_UTILS_H_

#include <map>
#include <sstream>
#include <string>
#include <vector>

class Utils {
 public:
  static void strTrim(std::string& str, const char* charset);
  static bool isContain(const std::string& str, int (*is_func)(int));
  static bool isContain(const std::string& str, const std::string charset);
  static bool isContainsOnly(const std::string& str, int (*is_func)(int));
  static bool isContainsOnly(const std::string& str, const std::string charset);
  static void toLowerString(std::string& str);
  static std::string toLower(const std::string& str);
  static void toUpperString(std::string& str);
  static std::string toUpper(const std::string& str);
  static bool strToSize_t(const std::string& str, size_t& num, int base);
  static bool isStartsWith(const std::string& str, const std::string& prefix);
  static bool isEndsWith(const std::string& str, const std::string& suffix);
  static bool isStartsWithCaseInsensitive(const std::string& str,
                                          const std::string& prefix);
  static std::string popFrontSubstr(std::string& str, std::size_t n);
  static std::string getExtension(const std::string& file);

  template <typename T>
  static bool parseValue(const std::string& str, T& value) {
    std::istringstream ss(str);
    return !(ss >> value).fail() && ss.eof();
  }

  static std::vector<std::string> split(const std::string& str, char separator);
  static std::string joinStrings(const std::vector<std::string>& strings,
                                 std::string delimiter);
  static bool isSameValueCaseInsensitive(
      const std::map<std::string, std::string>& mp, const std::string& key,
      const std::string& value);
  static std::string uintToString(unsigned int val);
  static bool hasContentInMap(const std::map<std::string, std::string>& mp,
                              const std::string& key);
  static std::string concatWithSlash(const std::string& s1,
                                      const std::string& s2);

 private:
  Utils(void);
  ~Utils(void);
  Utils(const Utils&);
  void operator=(const Utils&);
};

#endif
