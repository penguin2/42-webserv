#ifndef WEBSERV_HTTP_MOCK_H
#define WEBSERV_HTTP_MOCK_H

#include <sstream>
#include <string>

#include "ConnectionState.hpp"

class HttpMock {
 public:
  HttpMock();
  ~HttpMock();

  connection::State httpHandler();
  void appendClientData(const std::string& data);
  std::string getResponse() const;

 private:
  typedef enum eHttpState { RECV, SEND } HttpState;
  HttpState state_;
  std::string client_data_;
  std::stringstream raw_response_data_;

  HttpMock(const HttpMock&);
  HttpMock& operator=(const HttpMock&);

  bool checkResponse() const;
  void makeResponse();
};

#endif
