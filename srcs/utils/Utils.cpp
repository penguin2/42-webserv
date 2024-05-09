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

std::string Utils::toLower(const std::string &str) {
  std::string cpy(str);
  Utils::toLowerString(cpy);
  return cpy;
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

std::string Utils::getExtension(const std::string &file) {
  size_t pos_last_of_period = file.find_last_of('.');
  // '.'がない || '.'よりも後ろに'/'
  if (pos_last_of_period == std::string::npos ||
      file.find('/', pos_last_of_period) != std::string::npos)
    return "";
  return file.substr(pos_last_of_period + 1);
}

std::vector<std::string> Utils::split(const std::string &str, char separator) {
  std::vector<std::string> strings;
  std::string buf;
  std::stringstream ss(str);

  while (std::getline(ss, buf, separator)) {
    if (!buf.empty()) {
      strings.push_back(buf);
    }
  }
  return strings;
}

std::string Utils::joinStrings(const std::vector<std::string> &strings,
                               std::string delimiter) {
  std::stringstream ss;
  for (std::vector<std::string>::const_iterator it = strings.begin();
       it != strings.end();) {
    ss << *it;
    ++it;
    if (it != strings.end()) ss << delimiter;
  }
  return ss.str();
}

bool Utils::isSameValueCaseInsensitive(
    const std::map<std::string, std::string> &mp, const std::string &key,
    const std::string &value) {
  std::map<std::string, std::string>::const_iterator it = mp.find(key);
  if (it == mp.end()) return false;
  return Utils::toLower(it->second) == Utils::toLower(value);
}

std::string Utils::uintToString(unsigned int val) {
  std::stringstream ss;
  ss << std::dec << val;
  return ss.str();
}
