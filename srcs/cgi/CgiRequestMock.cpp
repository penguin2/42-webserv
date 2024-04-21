#include "CgiRequestMock.hpp"

#include <cstring>

#include "Time.hpp"

CgiRequestMock::CgiRequestMock() : directory_(NULL), filename_(NULL) {}

CgiRequestMock::~CgiRequestMock() {
  delete[] filename_;
  delete[] directory_;
}

const std::map<std::string, std::string>& CgiRequestMock::getEnvVars() const {
  return env_vars_;
}

std::string CgiRequestMock::getMessage() const { return message_; }

char* CgiRequestMock::getDirectory() const { return directory_; }
char* CgiRequestMock::getFilename() const { return filename_; }

void CgiRequestMock::addEnvVar(const std::string& key,
                               const std::string& value) {
  env_vars_[key] = value;
}

void CgiRequestMock::setMessage(const std::string& message) {
  message_ = message;
}

CgiRequestMock* CgiRequestMock::createCgiRequest(
    const std::string& request_message) {
  if (Time::getCurrentClockTime().getTimeNsec() & 1) return NULL;

  CgiRequestMock* new_cgi_request = new CgiRequestMock();

  new_cgi_request->addEnvVar("sample-key", "sample-value");
  new_cgi_request->addEnvVar("hello", "world");
  new_cgi_request->setMessage("request_message:\n" + request_message);

  std::string tmp_directory = "./";
  new_cgi_request->directory_ = new char[tmp_directory.size() + 1];
  std::strcpy(new_cgi_request->directory_, tmp_directory.c_str());

  std::string tmp_filename = "./test.py";
  new_cgi_request->filename_ = new char[tmp_filename.size() + 1];
  std::strcpy(new_cgi_request->filename_, tmp_filename.c_str());

  return new_cgi_request;
}
