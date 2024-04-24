#include "CgiRequestMock.hpp"

#include <cstring>

#include "Time.hpp"

CgiRequestMock::CgiRequestMock() {}

CgiRequestMock::~CgiRequestMock() {}

const std::map<std::string, std::string>& CgiRequestMock::getEnvVars() const {
  return env_vars_;
}

std::string CgiRequestMock::getMessage() const { return message_; }

std::string CgiRequestMock::getDirectory() const { return directory_; }

std::string CgiRequestMock::getFilename() const { return filename_; }

void CgiRequestMock::addEnvVar(const std::string& key,
                               const std::string& value) {
  env_vars_[key] = value;
}

void CgiRequestMock::setMessage(const std::string& message) {
  message_ = message;
}

void CgiRequestMock::setDirectory(std::string directory) {
  directory_ = directory;
}

void CgiRequestMock::setFilename(std::string filename) { filename_ = filename; }

CgiRequestMock* CgiRequestMock::createCgiRequest(
    const std::string& request_message) {
  if (Time::getCurrentClockTime().getTimeNsec() & 1) return NULL;

  CgiRequestMock* new_cgi_request = new CgiRequestMock();

  new_cgi_request->addEnvVar("sample-key", "sample-value");
  new_cgi_request->addEnvVar("hello", "world");
  new_cgi_request->setMessage("request_message:\n" + request_message);

  new_cgi_request->setDirectory("./");
  new_cgi_request->setFilename("./test.py");

  return new_cgi_request;
}
