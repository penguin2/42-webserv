#ifndef WEBSERV_CGIREQUEST_H_
#define WEBSERV_CGIREQUEST_H_

#include <map>
#include <string>

#include "Request.hpp"
#include "SocketAddress.hpp"

class CgiRequest {
 public:
  CgiRequest();
  ~CgiRequest();

  const std::map<std::string, std::string>& getEnvVars() const;
  std::string getMessage() const;
  std::string getDirectory() const;
  std::string getFilename() const;

  void addEnvVar(const std::string& key, const std::string& value);
  void setMessage(const std::string& message);
  void setDirectory(std::string directory);
  void setFilename(std::string filename);

  static CgiRequest* createCgiRequest(const Request& request,
                                      SocketAddress peer_addr);

 private:
  std::map<std::string, std::string> env_vars_;
  std::string message_;
  std::string directory_;
  std::string filename_;

  void insertContentLengthAndBodyIfHasHeader(const Request& request);
  void insertContentTypeIfHasHeader(const Request& request);
  void insertServerName(const Request& request);
  void insertScriptName(const Request& request);
  void insertHTTPHeaders(const Request& request);

  CgiRequest(const CgiRequest&);
  CgiRequest& operator=(const CgiRequest&);
};

#endif
