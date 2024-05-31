#ifndef WEBSERV_REQUESTDATA_H_
#define WEBSERV_REQUESTDATA_H_

#include <map>
#include <string>

#include "Uri.hpp"
#include "config/ServerConfig.hpp"

class RequestData {
 public:
  RequestData(void);
  ~RequestData(void);

  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void insertHeader(std::string& line);
  void appendBody(const std::string& data);
  void setServerConfig(const ServerConfig& server_conf);
  void overwritePath(const std::string& new_path);

  const std::string& getMethod(void) const;
  const Uri& getUri(void) const;
  const std::string& getVersion(void) const;
  const std::map<std::string, std::string>& getHeaders(void) const;
  const std::string& getBody(void) const;
  const ServerConfig* getServerConfig(void) const;

 private:
  std::string method_;
  Uri uri_;
  std::string version_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  const ServerConfig* server_config;

  RequestData(const RequestData&);
  void operator=(const RequestData&);
};

#endif
