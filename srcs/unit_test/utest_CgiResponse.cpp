#include <gtest/gtest.h>

#include "CgiResponse.hpp"
#include "ServerException.hpp"

using namespace std;

// 正常系のTEST
void testParse(string cgi_raw_string) {
  CgiResponse cgi_response;
  ASSERT_NO_THROW(cgi_response.parse(cgi_raw_string));
}

// 異常系で例外がthrowされるかをテスト
void testParseError(string cgi_raw_string) {
  CgiResponse cgi_response;
  ASSERT_THROW(cgi_response.parse(cgi_raw_string), ServerException);
}

TEST(CgiResponse, PARSE) {
  testParse("Location: /index.html\n\n");
  testParse("Location: http://localhost\n\n");
  testParse("Location: http://localhost/?q=50#fragment\n\n");
  testParse("Status: 200 OK\n\n");
  testParse("Status: 999 OKOK\n\n");
  testParse("Status: 444 Four Four Four\n\n");
  testParse("MY-Header: MyValue\n\n");
  testParse("Content-Type: text/html\n\n");
  testParse("Content-Type: my/type\n\n");
  testParse("Content-Type: my/type; charset=UTF-8\n\n");
  testParse("Location:\nLocation:/index.html\n\n");
  testParse("status: \nstatus: 200 OK\n\n");
  testParse("Content-Type:  \nContent-Type: n/n \n\n");

  // 改行のみの場合はCgiResponseHandlerでエラーになります
  testParse("\n");
  testParse("\nBODY");
  testParse("");
  testParse("Location: /abc");
  testParse("Location: /abc\n");
  testParse("Status: 200 OK\n");
  testParse("MY-Header: MyValue\n");
  testParse(" MY-Header: MyValue\n");
}

TEST(CgiResponse, ParseThrowServerException) {
  testParseError("MyHeader\n\n");
  testParseError(":MyValue\n\n");
  testParseError(":\n\n");
  testParseError("MyHeader : MyValue\n\n");
  // LocationHeaderのフォーマットは正しくなくてはならない
  testParseError("Location: 1\n\n");
  testParseError("Location: http://\n\n");
  testParseError("Location: /../index.html\n\n");
  testParseError("Location: http://localhost/../index.html\n\n");
  // 課題上、HTTPSに対応しないのでリダイレクト先もHTTPSに対応しない
  testParseError("Location: https://localhost/\n\n");
  // StatusHeaderのフォーマットは正しくなくてはならない
  testParseError("Status: 3333 ERROR\n\n");
  testParseError("Status: 33 ERROR\n\n");
  testParseError("Status: ERROR\n\n");
  testParseError("Status: 200\n\n");
  testParseError("Status: 200 \n\n");
  testParseError("Status: 200 \n\nBODY");
  // Content-Typeのフォーマットは正しくなくてはならない
  testParseError("Content-Type: text \n\n");
  testParseError("Content-Type: text/ \n\n");
  testParseError("Content-Type:  /html \n\n");
  // phrase is TAB
  testParseError("Status: 301	 \n\n");
  testParseError("Status: 301 \v \n\n");
  // Multiple CGI field
  testParseError("Location: /index.html\nLocation: /index.html\n\n");
  testParseError("Status: 200 OK\nstatus: 302 Found\n\n");
  testParseError("Content-Type: a/a\ncONTENT-tYPE: b/b\n\n");
  // int overflow
  testParseError("status: 4294967496 KO\n\n");
}
