#ifndef WEBSERV_COOKIE_H_
#define WEBSERV_COOKIE_H_

#include <string>

class Cookie {
 public:
  Cookie(void);
  ~Cookie(void);
  Cookie(const Cookie&);
  const Cookie& operator=(const Cookie&);

  void setName(const std::string& cookie_name);
  void setValue(const std::string& cookie_value);
  bool setAndCheckExpires(const std::string& expires);
  bool setAndCheckMaxAge(const std::string& max_age);
  bool setAndCheckDomain(const std::string& domain);
  bool setAndCheckPath(const std::string& path);
  bool setSecure(bool is_secure);
  bool setHttpOnly(bool is_http_only);

  const std::string& getName(void) const;
  const std::string& getValue(void) const;
  const std::string& getExpires(void) const;
  const std::string& getMaxAge(void) const;
  const std::string& getDomain(void) const;
  const std::string& getPath(void) const;
  bool isSecure(void) const;
  bool isHttpOnly(void) const;

 private:
  std::string name_;
  std::string value_;
  std::string expires_;
  std::string max_age_;
  std::string domain_;
  std::string path_;
  bool secure_;
  bool httponly_;
};

#endif
