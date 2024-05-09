#include "UriUtils.hpp"

#include <sstream>
#include <vector>

#include "ServerException.hpp"
#include "Utils.hpp"

bool UriUtils::decodeUrlEncoding(std::string &str) {
  std::string decoded_str;
  size_t num;
  size_t idx = 0;
  while (idx < str.size()) {
    if (str[idx] == '%') {
      if (str.size() <= (idx + 2)) {
        return false;
      } else {
        std::string hex_2_characters(str.substr(idx + 1, 2));
        if (Utils::strToSize_t(hex_2_characters, num, 16) == false)
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

bool UriUtils::isIPv4Address(const std::string &str) {
  std::istringstream iss(str);
  std::string segment;
  std::vector<std::string> segments;

  while (std::getline(iss, segment, '.')) segments.push_back(segment);
  if (segments.size() != 4) return false;

  size_t num;
  for (std::vector<std::string>::const_iterator it = segments.begin();
       it != segments.end(); ++it) {
    if (it->size() == 0 || 3 < it->size()) return false;
    if (2 <= it->size() && (*it)[0] == '0') return false;
    if (Utils::strToSize_t(*it, num, 10) == false || 255 < num) return false;
  }
  return true;
}

int UriUtils::isUnreserved(int c) {
  return (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~');
}

int UriUtils::isSubDelims(int c) {
  std::string str;
  str.push_back(c);
  return Utils::isContain(str, "!$&'()*+,;=");
}

int UriUtils::isPctEncodingCharset(int c) {
  std::string str;
  str.push_back(c);
  return (std::isdigit(c) || Utils::isContain(str, "abcdefABCDEF") || c == '%');
}

int UriUtils::isRegName(int c) {
  return (isUnreserved(c) || isPctEncodingCharset(c) || isSubDelims(c));
}

int UriUtils::isUserInfoCharset(int c) { return (isRegName(c) || c == ':'); }

int UriUtils::isPathCharset(int c) {
  return (isRegName(c) || c == ':' || c == '@' || c == '/');
}

int UriUtils::isQueryCharset(int c) {
  return (isRegName(c) || c == ':' || c == '@' || c == '/' || c == '?');
}

std::string UriUtils::removeDotSegments(const std::string &path) {
  const bool is_finish_slash = (!path.empty() && path[path.size() - 1] == '/');
  std::vector<std::string> input_buffer = Utils::split(path, '/');
  std::vector<std::string> output_buffer;

  for (std::vector<std::string>::const_iterator it = input_buffer.begin();
       it != input_buffer.end(); ++it) {
    if (*it == ".") continue;
    if (*it == "..") {
      if (output_buffer.empty()) {
        throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
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
