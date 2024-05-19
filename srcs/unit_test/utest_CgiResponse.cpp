#include <gtest/gtest.h>

#include "CgiResponse.hpp"
#include "ServerException.hpp"

using namespace std;

// 正常系のTEST & PARSE終了かの判定
// (HeaderとBodyを分けるLFが存在すればパースを終了可能)
void testParse(string cgi_raw_string, bool expect) {
  CgiResponse cgi_response;
  bool parse_end;
  ASSERT_NO_THROW(parse_end = cgi_response.parse(cgi_raw_string));
  EXPECT_EQ(parse_end, expect);
}

// 異常系で例外がthrowされるかをテスト
void testParseThrowException(string cgi_raw_string) {
  CgiResponse cgi_response;
  ASSERT_THROW(cgi_response.parse(cgi_raw_string), ServerException);
}

TEST(CgiResponse, PARSE) {
  // CGI RAW STRING, EXPECT PARSE FINISH
  testParse("\n", true);
  testParse("Location: /index.html\n\n", true);
  testParse("Location: http://localhost\n\n", true);
  testParse("Location: http://localhost/?q=50#fragment\n\n", true);
  testParse("Status: 200 OK\n\n", true);
  testParse("Status: 999 OKOK\n\n", true);
  testParse("Status: 444 Four Four Four\n\n", true);
  testParse("MY-Header: MyValue\n\n", true);
  testParse("\nBODY", true);
  testParse("Content-Type: text/html\n\n", true);
  testParse("Content-Type: my/type\n\n", true);
  testParse("Content-Type: my/type; charset=UTF-8\n\n", true);

  testParse("", false);
  testParse("Location: /abc", false);
  testParse("Location: /abc\n", false);
  testParse("Status: 200 OK\n", false);
  testParse("MY-Header: MyValue\n", false);
}

TEST(CgiResponse, ParseThrowServerException) {
  // CGI RAW STRING
  testParseThrowException("Location: 1\n\n");
  testParseThrowException("Location: http://\n\n");
  testParseThrowException("Location: http://localhost/../index.html\n\n");
  testParseThrowException("Location: /../index.html\n\n");
  testParseThrowException("Status: 3333 ERROR\n\n");
  testParseThrowException("Status: 33 ERROR\n\n");
  testParseThrowException("Status: ERROR\n\n");
  testParseThrowException("Status: 200\n\n");
  testParseThrowException("Status: 200 \n\n");
  testParseThrowException("Status: 200 \n\nBODY");
  testParseThrowException("Content-Type: text \n\n");
  testParseThrowException("Content-Type: text/ \n\n");
  testParseThrowException("Content-Type:  /html \n\n");
}
