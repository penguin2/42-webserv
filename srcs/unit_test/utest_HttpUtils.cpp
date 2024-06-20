#include <gtest/gtest.h>

#include <string>

#include "utils/HttpUtils.hpp"

void testGenerateContentType(const std::string& path, const char* expect) {
  EXPECT_STREQ(http_utils::convertPathToContentType(path).c_str(), expect);
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
  EXPECT_EQ(http_utils::isKeepConnection(100), false);
  EXPECT_EQ(http_utils::isKeepConnection(101), false);
  EXPECT_EQ(http_utils::isKeepConnection(200), true);
  EXPECT_EQ(http_utils::isKeepConnection(201), true);
  EXPECT_EQ(http_utils::isKeepConnection(202), false);
  EXPECT_EQ(http_utils::isKeepConnection(203), false);
  EXPECT_EQ(http_utils::isKeepConnection(204), true);
  EXPECT_EQ(http_utils::isKeepConnection(205), false);
  EXPECT_EQ(http_utils::isKeepConnection(206), false);
  EXPECT_EQ(http_utils::isKeepConnection(300), false);
  EXPECT_EQ(http_utils::isKeepConnection(301), true);
  EXPECT_EQ(http_utils::isKeepConnection(302), true);
  EXPECT_EQ(http_utils::isKeepConnection(303), true);
  EXPECT_EQ(http_utils::isKeepConnection(304), false);
  EXPECT_EQ(http_utils::isKeepConnection(307), true);
  EXPECT_EQ(http_utils::isKeepConnection(308), true);
  EXPECT_EQ(http_utils::isKeepConnection(400), false);
  EXPECT_EQ(http_utils::isKeepConnection(401), false);
  EXPECT_EQ(http_utils::isKeepConnection(402), false);
  EXPECT_EQ(http_utils::isKeepConnection(403), false);
  EXPECT_EQ(http_utils::isKeepConnection(404), false);
  EXPECT_EQ(http_utils::isKeepConnection(405), false);
  EXPECT_EQ(http_utils::isKeepConnection(406), false);
  EXPECT_EQ(http_utils::isKeepConnection(407), false);
  EXPECT_EQ(http_utils::isKeepConnection(408), false);
  EXPECT_EQ(http_utils::isKeepConnection(409), false);
  EXPECT_EQ(http_utils::isKeepConnection(410), false);
  EXPECT_EQ(http_utils::isKeepConnection(411), false);
  EXPECT_EQ(http_utils::isKeepConnection(412), false);
  EXPECT_EQ(http_utils::isKeepConnection(413), false);
  EXPECT_EQ(http_utils::isKeepConnection(414), false);
  EXPECT_EQ(http_utils::isKeepConnection(415), false);
  EXPECT_EQ(http_utils::isKeepConnection(416), false);
  EXPECT_EQ(http_utils::isKeepConnection(417), false);
  EXPECT_EQ(http_utils::isKeepConnection(421), false);
  EXPECT_EQ(http_utils::isKeepConnection(425), false);
  EXPECT_EQ(http_utils::isKeepConnection(426), false);
  EXPECT_EQ(http_utils::isKeepConnection(428), false);
  EXPECT_EQ(http_utils::isKeepConnection(429), false);
  EXPECT_EQ(http_utils::isKeepConnection(431), false);
  EXPECT_EQ(http_utils::isKeepConnection(451), false);
  EXPECT_EQ(http_utils::isKeepConnection(500), false);
  EXPECT_EQ(http_utils::isKeepConnection(501), false);
  EXPECT_EQ(http_utils::isKeepConnection(502), false);
  EXPECT_EQ(http_utils::isKeepConnection(503), false);
  EXPECT_EQ(http_utils::isKeepConnection(504), false);
  EXPECT_EQ(http_utils::isKeepConnection(505), false);
  EXPECT_EQ(http_utils::isKeepConnection(509), false);
  EXPECT_EQ(http_utils::isKeepConnection(510), false);
  EXPECT_EQ(http_utils::isKeepConnection(511), false);
  EXPECT_EQ(http_utils::isKeepConnection(0), false);
}

TEST(HttpUtils, IS_REDIRECT_STATUS_CODE) {
  EXPECT_EQ(http_utils::isRedirectStatusCode(300), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(301), true);
  EXPECT_EQ(http_utils::isRedirectStatusCode(302), true);
  EXPECT_EQ(http_utils::isRedirectStatusCode(303), true);
  EXPECT_EQ(http_utils::isRedirectStatusCode(304), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(305), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(306), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(307), true);
  EXPECT_EQ(http_utils::isRedirectStatusCode(308), true);
  EXPECT_EQ(http_utils::isRedirectStatusCode(309), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(310), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(0), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(400), false);
  EXPECT_EQ(http_utils::isRedirectStatusCode(500), false);
}

TEST(UttpUtils, IS_FULL_DATE_RFC1123) {
  EXPECT_TRUE(http_utils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 GMT"));
  EXPECT_TRUE(http_utils::isFullDateRFC1123("Wed, 99 Dec 9999 99:99:99 GMT"));
  EXPECT_TRUE(http_utils::isFullDateRFC1123("Fri, 00 Nov 0000 00:00:00 GMT"));

  EXPECT_FALSE(http_utils::isFullDateRFC1123(""));
  EXPECT_FALSE(http_utils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 GMT "));
  EXPECT_FALSE(http_utils::isFullDateRFC1123("Mon,11 Jan 2000 12:34:56 GMT"));
  EXPECT_FALSE(http_utils::isFullDateRFC1123("mon, 11 jan 2000 12:34:56 GMT"));
  EXPECT_FALSE(http_utils::isFullDateRFC1123("Mon, 11 Jan 2000 12:34:56 gmt"));
}
