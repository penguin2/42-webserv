#include <gtest/gtest.h>

#include "CgiResponse.hpp"
#include "CgiResponseHandler.hpp"
#include "Request.hpp"
#include "ResponseData.hpp"
#include "ServerException.hpp"
#include "config/ServerConfig.hpp"

using namespace std;

class RequestMock {
 public:
  RequestMock(void) {
    string buffer("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    this->server_conf_ = new ServerConfig;
    this->request_.parse(buffer);
    this->request_.setServerConfig(*this->server_conf_);
  }

  ~RequestMock(void) { delete this->server_conf_; }
  Request& getRequest(void) { return request_; }

 private:
  Request request_;
  ServerConfig* server_conf_;
};

void testConvertCgi2Http(string cgi_response_string) {
  RequestMock request;
  CgiResponse cgi_response;
  ASSERT_NO_THROW(cgi_response.parse(cgi_response_string))
      << cgi_response_string;

  ASSERT_NO_THROW(CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
      request.getRequest(), cgi_response.getResponseData()));
}

void testConvertCgi2HttpFailure(string cgi_response_string) {
  RequestMock request;
  CgiResponse cgi_response;
  ASSERT_NO_THROW(cgi_response.parse(cgi_response_string));

  ASSERT_THROW(CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
                   request.getRequest(), cgi_response.getResponseData()),
               ServerException);
}

TEST(CgiResponseHandler, ConvertCgi2Http) {
  // DocumentResonse
  testConvertCgi2Http("Content-Type: text/html\nStatus: 200 FFFF\n\nabc");
  // LocalRedirectResponse
  testConvertCgi2Http("Location: /\n\n");
  testConvertCgi2Http("Location: /././././index.html\n\n");
  testConvertCgi2Http("Location: /aaa/../index.html\n\n");
  // ClientRedirectResponse
  testConvertCgi2Http("Location: http://localhost/index.html\n\n");
  testConvertCgi2Http("Location: http://localhost/?q=50\n\n");
  testConvertCgi2Http("Location: http://localhost/?c=%20#fragment\n\n");
  // ClientRedirectResponseWithDocument
}

TEST(CgiResponseHandler, ConvertCgi2HttpFailure) {
  testConvertCgi2HttpFailure("");
  testConvertCgi2HttpFailure("ABC: ABC\n\n");
  testConvertCgi2HttpFailure("Location: /\nA:a\n\n");
  testConvertCgi2HttpFailure("Location: http://localhost/\nMyHeader:1\n\n");
}
