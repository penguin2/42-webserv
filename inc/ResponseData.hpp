#ifndef WEBSERV_RESPONSEDATA_H_
#define WEBSERV_RESPONSEDATA_H_

#include <map>
#include <string>

#include "Cookie.hpp"
#include "CookieManager.hpp"

class ResponseData {
 public:
  ResponseData(void);
  ~ResponseData(void);

  size_t getStatusCode(void) const;
  const std::string &getPhrase(void) const;
  const std::map<std::string, std::string> &getHeaders(void) const;
  const std::string &getBody(void) const;
  const std::map<std::string, Cookie> &getCookies(void) const;

  void setStatusCode(size_t code);
  void setPhrase(const std::string &phrase);
  void appendBody(const std::string &str);
  void insertHeader(const std::string &key, const std::string &value);
  void eraseHeader(const std::string &key);
  bool insertCookie(const std::string &set_cookie_header_value);
  void clearBody(void);

 private:
  size_t status_code_;
  std::string phrase_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  CookieManager cookie_manager_;

  ResponseData(const ResponseData &);
  void operator=(const ResponseData &);
};

#endif
