#ifndef WEBSERV_HTTP_MOCK_H
#define WEBSERV_HTTP_MOCK_H

#include <sstream>
#include <string>

#include "CgiRequestMock.hpp"
#include "ConnectionState.hpp"

class HttpMock {
 public:
  HttpMock();
  ~HttpMock();

  connection::State httpHandler();
  void appendClientData(const std::string& data);
  std::string getResponse() const;

  CgiRequestMock* getCgiRequest() const;
  void setCgiResponseMessage(const std::string& message);

 private:
  typedef enum eHttpState { RECV, SEND, CGI } HttpState;
  HttpState state_;
  std::string client_data_;
  std::stringstream raw_response_data_;

  CgiRequestMock* cgi_request_;
  std::string cgi_response_message_;

  static const std::string kCgiCheckString;

  HttpMock(const HttpMock&);
  HttpMock& operator=(const HttpMock&);

  bool checkResponse() const;
  void makeResponse();

  bool isCgi() const;
  void makeCgiResponse();
  void clearCgiHandling();
};

typedef HttpMock Http;

#endif
