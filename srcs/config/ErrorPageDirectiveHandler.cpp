#include "./config/ErrorPageDirectiveHandler.hpp"

#include <set>
#include <stdexcept>

#include "UriUtils.hpp"
#include "Utils.hpp"

ErrorPageDirectiveHandler::ErrorPageDirectiveHandler() {
  this->directive_context_ = ConfigEnums::SERVER;
}

bool ErrorPageDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() < 4) {
    return false;
  }

  const size_t page_index = (tokens_.size() - 2);
  const std::string& page = tokens_[page_index];
  if (!Utils::isContainsOnly(page, UriUtils::isPathCharsetWithoutPctEncoding))
    return false;

  std::vector<std::string> unique_code_strings =
      tokensToUniqueCodeStrings(tokens_);
  try {
    codeStringsToStatusCodes(unique_code_strings);
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

std::vector<std::string> ErrorPageDirectiveHandler::tokensToUniqueCodeStrings(
    const std::vector<std::string>& tokens) {
  std::vector<std::string>::const_iterator code_begin = ++(tokens.begin());
  std::vector<std::string>::const_iterator code_end = --(--(tokens.end()));
  std::set<std::string> code_set(code_begin, code_end);

  return std::vector<std::string>(code_set.begin(), code_set.end());
}

std::vector<int> ErrorPageDirectiveHandler::codeStringsToStatusCodes(
    const std::vector<std::string>& code_strings) {
  std::vector<int> status_codes;

  for (std::vector<std::string>::const_iterator code_str = code_strings.begin();
       code_str != code_strings.end(); ++code_str) {
    size_t code;
    if (Utils::strToSize_t(*code_str, code, 10) == false ||
        (code < 100 || 999 < code)) {
      throw std::out_of_range("Not Status Code");
    }
    status_codes.push_back(code);
  }
  return status_codes;
}

void ErrorPageDirectiveHandler::setConfig() {
  log();
  ServerConfig& serverConfig = getServerConfig();
  const size_t page_index = (tokens_.size() - 2);
  const std::string& page = tokens_[page_index];

  std::vector<std::string> code_strings = tokensToUniqueCodeStrings(tokens_);
  std::vector<int> codes = codeStringsToStatusCodes(code_strings);

  for (std::vector<int>::const_iterator code = codes.begin();
       code != codes.end(); ++code) {
    serverConfig.addErrorPage(*code, page);
  }
}
