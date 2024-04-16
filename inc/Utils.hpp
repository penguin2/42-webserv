#ifndef WEBSERV_UTILS_H_
#define WEBSERV_UTILS_H_

#include <string>

class Utils {
 public:
  static void strTrim(std::string& str, const char* charset);
  static bool isContain(const std::string& str, int (*is_func)(int));
  static bool isContain(const std::string& str, const std::string charset);
  static bool isContainsOnly(const std::string& str, int (*is_func)(int));
  static bool isContainsOnly(const std::string& str, const std::string charset);
  static void toLowerString(std::string& str);
  static bool strToSize_t(const std::string& str, size_t& num, int base);
  static bool isStartsWith(const std::string& str, const std::string& prefix);
  static std::string popFrontSubstr(std::string& str, std::size_t n);

 private:
  Utils(void);
  ~Utils(void);
  Utils(const Utils&);
  void operator=(const Utils&);
};

#endif
