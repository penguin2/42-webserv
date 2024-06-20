#include "utils/UriUtils.hpp"

#include <algorithm>
#include <sstream>
#include <vector>

#include "HttpException.hpp"
#include "utils/Utils.hpp"

bool uri_utils::decodeUrlEncoding(std::string &str) {
  std::string decoded_str;
  size_t num;
  size_t idx = 0;
  while (idx < str.size()) {
    if (str[idx] == '%') {
      if (str.size() <= (idx + 2)) {
        return false;
      } else {
        std::string hex_2_characters(str.substr(idx + 1, 2));
        if (utils::strToSize_t(hex_2_characters, num, 16) == false)
          return false;
      }
      idx += 2;
    } else {
      num = static_cast<size_t>(str[idx]);
    }
    decoded_str.push_back(static_cast<char>(num));
    idx++;
  }
  str = decoded_str;
  return true;
}

bool uri_utils::isIPv4Address(const std::string &str) {
  std::istringstream iss(str);
  std::string segment;
  std::vector<std::string> segments = utils::split(str, '.');

  if (std::count(str.begin(), str.end(), '.') != 3) return false;
  if (segments.size() != 4) return false;

  size_t num;
  for (std::vector<std::string>::const_iterator it = segments.begin();
       it != segments.end(); ++it) {
    if (it->size() == 0 || 3 < it->size()) return false;
    if (2 <= it->size() && (*it)[0] == '0') return false;
    if (utils::strToSize_t(*it, num, 10) == false || 255 < num) return false;
  }
  return true;
}

int uri_utils::isUnreserved(int c) {
  return (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~');
}

int uri_utils::isSubDelims(int c) {
  std::string str;
  str.push_back(c);
  return utils::isContain(str, "!$&'()*+,;=");
}

int uri_utils::isPctEncodingCharset(int c) {
  std::string str;
  str.push_back(c);
  return (std::isdigit(c) || utils::isContain(str, "abcdefABCDEF") || c == '%');
}

int uri_utils::isRegName(int c) {
  return (isUnreserved(c) || isPctEncodingCharset(c) || isSubDelims(c));
}

int uri_utils::isRegNameWithoutPctEncoding(int c) {
  return (isUnreserved(c) || isSubDelims(c));
}

int uri_utils::isUserInfoCharset(int c) { return (isRegName(c) || c == ':'); }

int uri_utils::isPathCharset(int c) {
  return (isRegName(c) || c == ':' || c == '@' || c == '/');
}

int uri_utils::isPathCharsetWithoutPctEncoding(int c) {
  return (isUnreserved(c) || isSubDelims(c) || c == ':' || c == '@' ||
          c == '/');
}

int uri_utils::isQueryCharset(int c) {
  return (isRegName(c) || c == ':' || c == '@' || c == '/' || c == '?');
}

std::string uri_utils::removeDotSegments(const std::string &path) {
  const bool is_finish_slash = (!path.empty() && path[path.size() - 1] == '/');
  std::vector<std::string> input_buffer = utils::split(path, '/');
  std::vector<std::string> output_buffer;

  for (std::vector<std::string>::const_iterator it = input_buffer.begin();
       it != input_buffer.end(); ++it) {
    if (*it == ".") continue;
    if (*it == "..") {
      if (output_buffer.empty()) {
        throw HttpException(HttpException::BAD_REQUEST,
                            "Detected access to parent directory");
      }
      output_buffer.pop_back();
    } else {
      output_buffer.push_back(*it);
    }
  }
  std::string removed_path;
  for (std::vector<std::string>::const_iterator it = output_buffer.begin();
       it != output_buffer.end(); ++it) {
    removed_path.append("/");
    removed_path.append(*it);
  }
  if (is_finish_slash) removed_path.append("/");
  return removed_path;
}
