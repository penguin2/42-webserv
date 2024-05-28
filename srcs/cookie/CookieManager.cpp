#include "CookieManager.hpp"

#include "Cookie.hpp"
#include "Utils.hpp"

CookieManager::CookieManager(void) {}
CookieManager::~CookieManager(void) {}

bool CookieManager::parseCookie(std::string set_cookie_header_value) {
  return true;
}

const std::map<std::string, Cookie>& CookieManager::getCookies(void) const {
  return this->cookies_;
}

const Cookie* CookieManager::searchCookie(const std::string& key) const {
  if (cookies_.find(key) != cookies_.end()) return &cookies_.find(key)->second;
  return NULL;
}
