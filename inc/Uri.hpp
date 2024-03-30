#ifndef WEBSERV_URI_H_
#define WEBSERV_URI_H_

#include <string>

class Uri {
 public:
  Uri(void);
  ~Uri(void);

  void parse(const std::string& uri);
  const std::string& getScheme(void) const;
  const std::string& getUserInfo(void) const;
  const std::string& getHost(void) const;
  const std::string& getPort(void) const;
  const std::string& getPath(void) const;
  const std::string& getQuery(void) const;
  const std::string& getFragment(void) const;

 private:
  std::string scheme_;
  std::string user_info_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string query_;
  std::string fragment_;

  Uri(const Uri&);
  void operator=(const Uri&);
};

#endif
