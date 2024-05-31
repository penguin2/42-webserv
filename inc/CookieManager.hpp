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

 private:
  std::map<std::string, Cookie> cookies_;

  bool parseCookieName(std::string& set_cookie_header_value, Cookie& cookie);
  bool parseCookieValue(std::string& set_cookie_header_value, Cookie& cookie);
  bool parseCookieAttributes(std::string& set_cookie_header_value,
                             Cookie& cookie);
  bool parseCookieAttribute(std::string attribute, Cookie& cookie);

  CookieManager(const CookieManager&);
  void operator=(const CookieManager&);
};

#endif
