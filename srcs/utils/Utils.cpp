#include "Utils.hpp"

#include <sstream>
#include <string>

void Utils::strTrim(std::string &str, const char *charset) {
  const size_t left = str.find_first_not_of(charset);
  if (left == std::string::npos)
    str = "";
  else {
    const size_t right = str.find_last_not_of(charset);
    str = str.substr(left, right - left + 1);
  }
}

bool Utils::isContain(const std::string &str, int (*is_func)(int)) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (is_func(*it)) return true;
  }
  return false;
}

bool Utils::isContain(const std::string &str, const std::string charset) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (charset.find(*it) != std::string::npos) return true;
  }
  return false;
}

bool Utils::isContainsOnly(const std::string &str, int (*is_func)(int)) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (!is_func(*it)) return false;
  }
  return true;
}

bool Utils::isContainsOnly(const std::string &str, const std::string charset) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (charset.find(*it) == std::string::npos) return false;
  }
  return true;
}

void Utils::toLowerString(std::string &str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
    if (std::isupper(*it)) *it = std::tolower(*it);
  }
}

// 符号なし 8 or 10 or 16進数対応
bool Utils::strToSize_t(const std::string &str, size_t &num, int base) {
  static const std::string string16 = "0123456789abcdef";
  std::string copy_str(str);
  Utils::toLowerString(copy_str);

  if (!Utils::isContainsOnly(copy_str, string16.substr(0, base))) return false;
  if (base != 8 && base != 10 && base != 16) return false;
  std::stringstream ss(copy_str);
  switch (base) {
    case 8:
      ss >> std::oct >> num;
      break;
    case 10:
      ss >> std::dec >> num;
      break;
    case 16:
      ss >> std::hex >> num;
      break;
    default:
      return false;
  }
  if (ss.fail() || ss.peek() != -1) return false;
  return true;
}

bool Utils::isStartsWith(const std::string &str, const std::string &prefix) {
  return (str.compare(0, prefix.size(), prefix) == 0);
}

std::string Utils::popFrontSubstr(std::string &str, std::size_t n) {
  std::string front_substr = str.substr(0, n);
  str.erase(0, n);
  return front_substr;
}
