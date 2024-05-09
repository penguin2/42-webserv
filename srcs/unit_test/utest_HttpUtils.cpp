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

TEST(HttpUtils, IS_MAINTAIN_CONNECTION) {
  EXPECT_EQ(HttpUtils::isMaintainConnection(100), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(101), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(200), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(201), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(202), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(203), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(204), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(205), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(206), true);

  EXPECT_EQ(HttpUtils::isMaintainConnection(300), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(301), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(302), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(303), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(304), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(307), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(308), true);

  EXPECT_EQ(HttpUtils::isMaintainConnection(400), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(401), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(402), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(403), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(404), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(405), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(406), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(407), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(408), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(409), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(410), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(411), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(412), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(413), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(414), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(415), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(416), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(417), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(421), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(425), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(426), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(428), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(429), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(431), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(451), true);

  EXPECT_EQ(HttpUtils::isMaintainConnection(500), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(501), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(502), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(503), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(504), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(505), false);
  EXPECT_EQ(HttpUtils::isMaintainConnection(509), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(510), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(511), true);
  EXPECT_EQ(HttpUtils::isMaintainConnection(0), true);
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
