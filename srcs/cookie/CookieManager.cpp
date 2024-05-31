#include "CookieManager.hpp"

#include <utility>

#include "Cookie.hpp"
#include "HttpUtils.hpp"
#include "Utils.hpp"

CookieManager::CookieManager(void) {}
CookieManager::~CookieManager(void) {}

bool CookieManager::parseCookie(std::string set_cookie_header_value) {
  Cookie cookie;
  if (parseCookieName(set_cookie_header_value, cookie) == false) return false;
  if (parseCookieValue(set_cookie_header_value, cookie) == false) return false;
  if (parseCookieAttributes(set_cookie_header_value, cookie) == false)
    return false;
  cookies_[cookie.getName()] = cookie;
  return true;
}

const std::map<std::string, Cookie>& CookieManager::getCookies(void) const {
  return this->cookies_;
}

bool CookieManager::parseCookieName(std::string& set_cookie_header_value,
                                    Cookie& cookie) {
  size_t pos_equal = set_cookie_header_value.find('=');
  const std::string name = set_cookie_header_value.substr(0, pos_equal);

  if (pos_equal == std::string::npos) return false;
  if (name.empty()) return false;
  if (!Utils::isContainsOnly(name, HttpUtils::isHeaderKeyChar)) return false;
  set_cookie_header_value.erase(0, pos_equal + 1);
  cookie.setName(name);
  return true;
}

bool CookieManager::parseCookieValue(std::string& set_cookie_header_value,
                                     Cookie& cookie) {
  std::string value;

  if (Utils::isStartsWith(set_cookie_header_value, "\"")) {
    size_t pos_close_d_quote = set_cookie_header_value.find('"', 1);
    if (pos_close_d_quote == std::string::npos) return false;
    std::string value_without_d_quote =
        set_cookie_header_value.substr(1, pos_close_d_quote - 1);
    value = set_cookie_header_value.substr(0, pos_close_d_quote + 1);
    if (!Utils::isContainsOnly(value_without_d_quote,
                               HttpUtils::isCookieValueChar)) {
      return false;
    }
  } else {
    size_t pos_end_of_value = set_cookie_header_value.find(';');
    if (pos_end_of_value == std::string::npos)
      pos_end_of_value = set_cookie_header_value.size();
    value = set_cookie_header_value.substr(0, pos_end_of_value);
    if (!Utils::isContainsOnly(value, HttpUtils::isCookieValueChar))
      return false;
  }
  set_cookie_header_value.erase(0, value.size());
  cookie.setValue(value);
  return true;
}

bool CookieManager::parseCookieAttributes(std::string& set_cookie_header_value,
                                          Cookie& cookie) {
  size_t pos_attribute_end;
  std::string attribute;

  while (set_cookie_header_value.empty() == false) {
    if (!Utils::isStartsWith(set_cookie_header_value, "; ")) return false;
    set_cookie_header_value.erase(0, 2);
    pos_attribute_end = set_cookie_header_value.find(';');
    if (pos_attribute_end == std::string::npos)
      pos_attribute_end = set_cookie_header_value.size();
    attribute = set_cookie_header_value.substr(0, pos_attribute_end);
    if (parseCookieAttribute(attribute, cookie) == false) return false;
    set_cookie_header_value.erase(0, pos_attribute_end);
  }
  return true;
}

bool CookieManager::parseCookieAttribute(std::string attribute,
                                         Cookie& cookie) {
  const std::string expires = "expires=";
  const std::string max_age = "max-age=";
  const std::string domain = "domain=";
  const std::string path = "path=";
  const std::string secure = "secure";
  const std::string httponly = "httponly";

  if (Utils::isStartsWithCaseInsensitive(attribute, expires))
    return cookie.setAndCheckExpires(attribute.substr(expires.size()));
  if (Utils::isStartsWithCaseInsensitive(attribute, max_age))
    return cookie.setAndCheckMaxAge(attribute.substr(max_age.size()));
  if (Utils::isStartsWithCaseInsensitive(attribute, domain))
    return cookie.setAndCheckDomain(attribute.substr(domain.size()));
  if (Utils::isStartsWithCaseInsensitive(attribute, path))
    return cookie.setAndCheckPath(attribute.substr(path.size()));
  if (Utils::toLower(attribute) == secure) return cookie.setSecure(true);
  if (Utils::toLower(attribute) == httponly) return cookie.setHttpOnly(true);
  return false;
}
