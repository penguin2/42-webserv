#include "utils/Utils.hpp"

#include <sstream>
#include <string>

void utils::strTrim(std::string& str, const char* charset) {
  const size_t left = str.find_first_not_of(charset);
  if (left == std::string::npos)
    str = "";
  else {
    const size_t right = str.find_last_not_of(charset);
    str = str.substr(left, right - left + 1);
  }
}

void utils::strLTrim(std::string& str, const char* charset) {
  const size_t left = str.find_first_not_of(charset);
  if (left == std::string::npos)
    str = "";
  else
    str = str.substr(left);
}

bool utils::isContain(const std::string& str, int (*is_func)(int)) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (is_func(*it)) return true;
  }
  return false;
}

bool utils::isContain(const std::string& str, const std::string charset) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (charset.find(*it) != std::string::npos) return true;
  }
  return false;
}

bool utils::isContainsOnly(const std::string& str, int (*is_func)(int)) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (!is_func(*it)) return false;
  }
  return true;
}

bool utils::isContainsOnly(const std::string& str, const std::string charset) {
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    if (charset.find(*it) == std::string::npos) return false;
  }
  return true;
}

void utils::toLowerString(std::string& str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
    if (std::isupper(*it)) *it = std::tolower(*it);
  }
}

std::string utils::toLower(const std::string& str) {
  std::string cpy(str);
  utils::toLowerString(cpy);
  return cpy;
}

void utils::toUpperString(std::string& str) {
  for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
    if (std::islower(*it)) *it = std::toupper(*it);
  }
}

std::string utils::toUpper(const std::string& str) {
  std::string cpy(str);
  utils::toUpperString(cpy);
  return cpy;
}

// 符号なし 8 or 10 or 16進数対応
bool utils::strToSize_t(const std::string& str, size_t& num, int base) {
  static const std::string string16 = "0123456789abcdef";
  std::string lower_str = utils::toLower(str);

  if (!utils::isContainsOnly(lower_str, string16.substr(0, base))) return false;
  if (base != 8 && base != 10 && base != 16) return false;
  std::stringstream ss(lower_str);
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

bool utils::isStartsWith(const std::string& str, const std::string& prefix) {
  return (str.compare(0, prefix.size(), prefix) == 0);
}

bool utils::isEndsWith(const std::string& str, const std::string& suffix) {
  if (str.size() < suffix.size()) return false;
  return (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
}

bool utils::isStartsWithCaseInsensitive(const std::string& str,
                                        const std::string& prefix) {
  return (utils::toLower(str).compare(0, prefix.size(),
                                      utils::toLower(prefix)) == 0);
}

std::string utils::popFrontSubstr(std::string& str, std::size_t n) {
  std::string front_substr = str.substr(0, n);
  str.erase(0, n);
  return front_substr;
}

std::string utils::getExtension(const std::string& file) {
  size_t pos_last_of_period = file.find_last_of('.');
  // '.'がない || '.'よりも後ろに'/'
  if (pos_last_of_period == std::string::npos ||
      file.find('/', pos_last_of_period) != std::string::npos)
    return "";
  return file.substr(pos_last_of_period + 1);
}

std::vector<std::string> utils::split(const std::string& str, char separator) {
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

std::string utils::joinStrings(const std::vector<std::string>& strings,
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

bool utils::isSameValueCaseInsensitive(
    const std::map<std::string, std::string>& mp, const std::string& key,
    const std::string& value) {
  std::map<std::string, std::string>::const_iterator it = mp.find(key);
  if (it == mp.end()) return false;
  return utils::toLower(it->second) == utils::toLower(value);
}

std::string utils::uintToString(unsigned int val) {
  std::stringstream ss;
  ss << std::dec << val;
  return ss.str();
}

bool utils::hasContentInMap(const std::map<std::string, std::string>& mp,
                            const std::string& key) {
  return (mp.find(key) != mp.end());
}

std::string utils::concatWithSlash(const std::string& s1,
                                   const std::string& s2) {
  if (isEndsWith(s1, "/") && isStartsWith(s2, "/")) return s1 + s2.substr(1);
  if (!isEndsWith(s1, "/") && !isStartsWith(s2, "/")) return s1 + "/" + s2;
  return s1 + s2;
}

std::string utils::findLongestString(const std::vector<std::string>& strings) {
  std::vector<std::string>::const_iterator longest_string_it = strings.begin();

  if (strings.empty()) return "";

  for (std::vector<std::string>::const_iterator it = strings.begin();
       it != strings.end(); ++it) {
    if (longest_string_it->size() < it->size()) longest_string_it = it;
  }
  return *longest_string_it;
}
