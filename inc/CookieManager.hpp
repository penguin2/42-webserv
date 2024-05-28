#ifndef WEBSERV_COOKIEMANAGER_H_
#define WEBSERV_COOKIEMANAGER_H_

#include <map>
#include <string>

#include "Cookie.hpp"

class CookieManager {
 public:
  CookieManager(void);
  ~CookieManager(void);
  // if (適切なCookie) { return true; } else { return false; }
  bool parseCookie(std::string set_cookie_header_value);
  const std::map<std::string, Cookie>& getCookies(void) const;
  const Cookie* searchCookie(const std::string& key) const;

 private:
  std::map<std::string, Cookie> cookies_;

  CookieManager(const CookieManager&);
  void operator=(const CookieManager&);
};

#endif
