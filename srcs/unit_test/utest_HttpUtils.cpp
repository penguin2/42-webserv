#include <gtest/gtest.h>

#include <string>

#include "HttpUtils.hpp"
#include "ServerException.hpp"

void testGeneratePage(const std::string& file, bool expect) {
  bool result;
  try {
    std::string page = HttpUtils::generatePage(file);
    result = true;
  } catch (ServerException& e) {
    result = false;
  }
  EXPECT_EQ(result, expect);
}

void testGenerateContentType(const std::string& path, const char* expect) {
  EXPECT_STREQ(HttpUtils::generateContentType(path).c_str(), expect);
}

TEST(HttpUtils, GENERATE_PAGE) {
  // file, directoryの判定や権限確認は対象外、ファイルが存在すれば成功
  testGeneratePage("/bin/ls", true);
  testGeneratePage("./Makefile", true);
  testGeneratePage("./inc/", true);
  testGeneratePage("./inc", true);
  testGeneratePage("/etc/nginx/", true);
  testGeneratePage("", false);
  testGeneratePage("./nofile", false);
  testGeneratePage("./nofile.html", false);
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
