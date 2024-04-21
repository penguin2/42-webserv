#ifndef WEBSERV_CGI_REQUEST_MOCK_H
#define WEBSERV_CGI_REQUEST_MOCK_H

#include <map>
#include <string>

class CgiRequestMock {
 public:
  CgiRequestMock();
  ~CgiRequestMock();

  const std::map<std::string, std::string>& getEnvVars() const;
  std::string getMessage() const;
  char* getDirectory() const;
  char* getFilename() const;

  void addEnvVar(const std::string& key, const std::string& value);
  void setMessage(const std::string& message);

  static CgiRequestMock* createCgiRequest(const std::string& requests_message);

 private:
  std::map<std::string, std::string> env_vars_;
  std::string message_;
  char* directory_;
  char* filename_;

  CgiRequestMock(const CgiRequestMock&);
  CgiRequestMock& operator=(const CgiRequestMock&);
};

typedef CgiRequestMock CgiRequest;

#endif
