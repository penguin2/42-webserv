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

// 正常系のTEST
void testConvertCgi2Http(string cgi_response_string) {
  RequestMock request;
  CgiResponse cgi_response;
  ASSERT_NO_THROW(cgi_response.parse(cgi_response_string));

  ASSERT_NO_THROW(CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
      request.getRequest(), cgi_response.getResponseData()));
}

// 異常系で例外がthrowされるかをテスト
void testConvertCgi2HttpError(string cgi_response_string) {
  RequestMock request;
  CgiResponse cgi_response;
  ASSERT_NO_THROW(cgi_response.parse(cgi_response_string));

  ASSERT_THROW(CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
                   request.getRequest(), cgi_response.getResponseData()),
               ServerException);
}

TEST(CgiResponseHandler, ConvertCgi2Http) {
  // DocumentResonse
  testConvertCgi2Http("Content-Type: text/html\n\nabc");
  testConvertCgi2Http("Content-Type: text/html\nStatus: 200 FFFF\n\nabc");
  testConvertCgi2Http("Content-Type: text/html\nStatus: 999 QQQQ\n\nabc");
  testConvertCgi2Http("Content-Type: text/html\nStatus: 200 \n\nabc");
  testConvertCgi2Http("Content-Type: text/html\nStatus: 999 \n\nabc");
  testConvertCgi2Http("MyHeader:1\nContent-Type: text/html\n\nabc");
  // LocalRedirectResponse
  testConvertCgi2Http("Location: /\n\n");
  testConvertCgi2Http("Location: /././././index.html\n\n");
  testConvertCgi2Http("location: /aaa/../index.html\n\n");
  testConvertCgi2Http("location: /DIR/\n\n");
  testConvertCgi2Http("location: /index.html?q=50&c=%20#fragment\n\n");
  testConvertCgi2Http("location: /DIR/\n MyHeder:1\n\n");
  // ClientRedirectResponse
  testConvertCgi2Http("Location: http://localhost/index.html\n\n");
  testConvertCgi2Http("Location: http://localhost/?q=50\n\n");
  testConvertCgi2Http("Location: http://localhost/?c=%20#fragment\n\n");
  testConvertCgi2Http("Location: http://user:pass@localhost/\n\n");
  // ClientRedirectResponseWithDocument
  testConvertCgi2Http(
      "Location: http://localhost/index.html\nStatus:301 "
      "F\nContent-Type:text/html\n\nabc");
  testConvertCgi2Http(
      "Location: http://localhost/?q=50\nStatus:302 "
      "F\nContent-Type:a/b\n\nabc");
  testConvertCgi2Http(
      "LOCATION: "
      "http://localhost/?c=%20#fragment\nsTATUS:303 "
      "Found\ncontent-type:1/2\n\nabc");
  testConvertCgi2Http(
      "status:307 FOUND\ncontent-type:a/a; charset=UTF=8\nLocation: "
      "http://user:pass@localhost/\n\nabc");
  testConvertCgi2Http(
      "status:308 FOUND\ncontent-type:a/a\nLocation:http://abc/\n\nabc");
}

TEST(CgiResponseHandler, ConvertCgi2HttpError) {
  testConvertCgi2HttpError("");
  testConvertCgi2HttpError("\n");
  testConvertCgi2HttpError("\n\n");
  testConvertCgi2HttpError("ABC: ABC\n\n");
  testConvertCgi2HttpError(" Location: /\n\n");
  // ONLY Status
  testConvertCgi2HttpError("Status: 200 OK\n\n");
  // Status and BODY
  testConvertCgi2HttpError("Status: 200 OK\n\nabc");
  // Location and Other-Header
  testConvertCgi2HttpError("Location: /\nA:a\n\n");
  testConvertCgi2HttpError("Location: http://localhost/\nA:a\n\n");
  // Location and BODY
  testConvertCgi2HttpError("Location: /\n\nabc");
  testConvertCgi2HttpError("Location: http://localhost/\n\nabc");
  // ONLY Content-Type
  testConvertCgi2HttpError("Content-Type: text/html\n\n");
  // Status and Location
  testConvertCgi2HttpError("Location:/\nStatus: 200 OK\n\n");
  testConvertCgi2HttpError("Location:http://abc/\nStatus: 200 OK\n\n");
  // Status and Location and BODY
  testConvertCgi2HttpError("Location:/\nStatus: 200 OK\n\nabc");
  testConvertCgi2HttpError("Location:http://abc/\nStatus: 200 OK\n\nabc");
  // Location and Content-Type
  testConvertCgi2HttpError("Location: /\nContent-Type: text/html\n\n");
  // Location and Content-Type and BODY
  testConvertCgi2HttpError("Location: /\nContent-Type: text/html\n\nabc");
  // INVALID REDIRECT STATUS CODE
  testConvertCgi2HttpError(
      "status:305 F\ncontent-type:a/a\nLocation:http://abc/\n\nabc");
}
