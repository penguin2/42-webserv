#include "./config/CgiExtDirectiveHandler.hpp"

#include <algorithm>
#include <set>

#include "Utils.hpp"

CgiExtDirectiveHandler::CgiExtDirectiveHandler() {
  this->directive_context_ = ConfigEnums::LOCATION;
}

bool CgiExtDirectiveHandler::isDirectiveValid() const {
  if (tokens_.size() < 3) {
    return false;
  }

  std::vector<std::string> unique_cgi_exts = tokensToUniqueCgiExts(tokens_);

  for (std::vector<std::string>::const_iterator ext = unique_cgi_exts.begin();
       ext != unique_cgi_exts.end(); ++ext) {
    if (!Utils::isStartsWith(*ext, ".") || *ext == ".") return false;
    if (Utils::isContain(*ext, "/")) return false;
    if (std::count(ext->begin(), ext->end(), '.') != 1) return false;
  }
  return true;
}

std::vector<std::string> CgiExtDirectiveHandler::tokensToUniqueCgiExts(
    const std::vector<std::string>& tokens) {
  std::vector<std::string>::const_iterator ext_begin = ++(tokens.begin());
  std::vector<std::string>::const_iterator ext_end = --(tokens.end());
  std::set<std::string> ext_set(ext_begin, ext_end);

  return std::vector<std::string>(ext_set.begin(), ext_set.end());
}

void CgiExtDirectiveHandler::setConfig() {
  log();
  LocationConfig& locationConfig = getLocationConfig();
  std::vector<std::string> unique_cgi_exts = tokensToUniqueCgiExts(tokens_);

  locationConfig.setCgiExts(unique_cgi_exts);
}
