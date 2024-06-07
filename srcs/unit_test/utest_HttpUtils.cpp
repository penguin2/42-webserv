#include <gtest/gtest.h>

#include <string>

#include "HttpUtils.hpp"
#include "ServerException.hpp"

void testGenerateContentType(const std::string& path, const char* expect) {
  EXPECT_STREQ(HttpUtils::convertPathToContentType(path).c_str(), expect);
}

TEST(HttpUtils, GENERATE_CONTENT_TYPE) {
  testGenerateContentType("data.csv", "text/csv");
  testGenerateContentType("index.html", "text/html");
  testGenerateContentType("style.css", "text/css");
  testGenerateContentType("next.js", "text/javascript");
  testGenerateContentType("setting.json", "application/json");
  testGenerateContentType("adobe.pdf", "application/pdf");
  testGenerateContentType("logo.png", "image/apng");
  testGenerateContentType("animal.jpeg", "image/jpeg");
  testGenerateContentType("food.gif", "image/gif");
  testGenerateContentType("sphere.svg", "image/svg+xml");
  testGenerateContentType("tar.zip", "application/zip");
  testGenerateContentType("youtube.mpeg", "video/mpeg");
  testGenerateContentType("none_type.ext", "text/plain");
  testGenerateContentType("", "text/plain");
  testGenerateContentType("Makefile", "text/plain");
  testGenerateContentType("main.cpp", "text/plain");
  testGenerateContentType(".hidden.files", "text/plain");
  testGenerateContentType("/ls", "text/plain");
}

TEST(HttpUtils, IS_KEEP_CONNECTION) {
  EXPECT_EQ(HttpUtils::isKeepConnection(100), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(101), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(200), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(201), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(202), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(203), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(204), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(205), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(206), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(300), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(301), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(302), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(303), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(304), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(307), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(308), true);
  EXPECT_EQ(HttpUtils::isKeepConnection(400), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(401), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(402), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(403), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(404), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(405), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(406), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(407), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(408), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(409), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(410), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(411), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(412), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(413), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(414), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(415), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(416), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(417), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(421), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(425), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(426), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(428), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(429), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(431), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(451), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(500), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(501), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(502), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(503), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(504), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(505), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(509), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(510), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(511), false);
  EXPECT_EQ(HttpUtils::isKeepConnection(0), false);
}

TEST(HttpUtils, IS_REDIRECT_STATUS_CODE) {
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(300), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(301), true);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(302), true);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(303), true);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(304), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(305), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(306), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(307), true);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(308), true);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(309), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(310), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(0), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(400), false);
  EXPECT_EQ(HttpUtils::isRedirectStatusCode(500), false);
}

TEST(UttpUtils, IS_FULL_DATE_RFC1123) {
  EXPECT_TRUE(HttpUtils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 GMT"));
  EXPECT_TRUE(HttpUtils::isFullDateRFC1123("Wed, 99 Dec 9999 99:99:99 GMT"));
  EXPECT_TRUE(HttpUtils::isFullDateRFC1123("Fri, 00 Nov 0000 00:00:00 GMT"));

  EXPECT_FALSE(HttpUtils::isFullDateRFC1123(""));
  EXPECT_FALSE(HttpUtils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 GMT "));
  EXPECT_FALSE(HttpUtils::isFullDateRFC1123("Mon,11 Jan 2000 12:34:56 GMT"));
  EXPECT_FALSE(HttpUtils::isFullDateRFC1123("mon, 11 jan 2000 12:34:56 GMT"));
  EXPECT_FALSE(HttpUtils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 gmt"));
}
