#ifndef WEBSERV_URI_H_
#define WEBSERV_URI_H_

#include <string>

class Uri {
 public:
  Uri(void);
  ~Uri(void);

  void parse(const std::string& uri);
  void overwriteAuthorityIfNotSet(const std::string& authority);

  const std::string& getScheme(void) const;
  const std::string& getUserInfo(void) const;
  const std::string& getHost(void) const;
  size_t getPort(void) const;
  const std::string& getPath(void) const;
  const std::string& getQuery(void) const;
  const std::string& getFragment(void) const;
  std::string buildAbsoluteUri(void) const;

 private:
  std::string scheme_;
  std::string user_info_;
  std::string host_;
  size_t port_;
  std::string path_;
  std::string query_;
  std::string fragment_;

  // Authority = UserInfo + Host + Port
  void parseAuthority(const std::string& authority);
  void parsePathQueryFragment(const std::string& path_query_fragment);

  void setAndCheckScheme(const std::string& scheme);
  void setAndCheckAndDecodeUserInfo(const std::string& user_info);
  void setAndCheckAndDecodeHost(const std::string& host);
  void setAndCheckPort(const std::string& port);
  void setAndCheckAndDecodePath(const std::string& path);
  void setAndCheckAndDecodeQuery(const std::string& query);
  void setFragment(const std::string& fragment);

  Uri(const Uri&);
  void operator=(const Uri&);
};

#endif
