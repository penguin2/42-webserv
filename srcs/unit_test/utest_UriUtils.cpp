#include <gtest/gtest.h>

#include "ServerException.hpp"
#include "UriUtils.hpp"

void testDecodeUrlEncoding(const char* target, const char* expect_str,
                           bool expect) {
  std::string target_string(target);
  bool ret = UriUtils::decodeUrlEncoding(target_string);
  EXPECT_EQ(ret, expect);
  if (ret == true) EXPECT_STREQ(target_string.c_str(), expect_str);
}

void testRemoveDotSegments(const char* target, const char* expect_str,
                           bool expect_no_throw) {
  if (expect_no_throw) {
    std::string result_str;
    ASSERT_NO_THROW((result_str = UriUtils::removeDotSegments(target)));
    EXPECT_STREQ(result_str.c_str(), expect_str);
  } else
    ASSERT_THROW(UriUtils::removeDotSegments(target), ServerException);
}

TEST(UriUtils, DECODE_URL_ENCODING_SUCCESS) {
  // string expect(string) expect(bool)
  testDecodeUrlEncoding("", "", true);
  testDecodeUrlEncoding("%20", " ", true);
  testDecodeUrlEncoding("%2f", "/", true);
  testDecodeUrlEncoding("%2F", "/", true);
  testDecodeUrlEncoding("%2F%69%6e%64%65%78%2E%68%74%6D%6c", "/index.html",
                        true);
  testDecodeUrlEncoding("20", "20", true);
  testDecodeUrlEncoding("%E3%82%A8%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89",
                        "エンコード", true);
  testDecodeUrlEncoding(
      "%E3%82%A6%E3%82%A3%E3%82%AD%E3%83%9A%E3%83%87%E3%82%A3%E3%82%A2",
      "ウィキペディア", true);
  testDecodeUrlEncoding(
      "http%3A%2F%2Fsample.com%3A80%2F%E3%82%A4%E3%83%B3%E3%83%87%E3%83%83%E3%"
      "82%AF%E3%82%B9.html%3F%E3%82%AF%E3%82%AA%E3%83%AA%E3%83%86%E3%82%A3%E3%"
      "83%BC%3D50%25",
      "http://sample.com:80/インデックス.html?クオリティー=50%", true);
  testDecodeUrlEncoding(
      "http://sample.com/"
      "%E5%BE%8C%E3%82%8D%E3%81%AB%E5%85%A8%E8%A7%92%E7%A9%BA%E7%99%BD%E3%81%"
      "8C%E3%81%82%E3%82%8B%E3%80%80.html",
      "http://sample.com/後ろに全角空白がある　.html", true);
}

TEST(UriUtils, DECODE_URL_ENCODING_ERROR) {
  // if (expect(bool) == false): expect(string) = -1;
  testDecodeUrlEncoding("%2", "-1", false);
  testDecodeUrlEncoding("%2%20", "-1", false);
  testDecodeUrlEncoding("%ZX%20", "-1", false);
  testDecodeUrlEncoding("%zx%20", "-1", false);
}

TEST(UriUtils, IS_IPV4_ADDRESS) {
  EXPECT_EQ(UriUtils::isIPv4Address("0.0.0.0"), true);
  EXPECT_EQ(UriUtils::isIPv4Address("127.0.0.1"), true);
  EXPECT_EQ(UriUtils::isIPv4Address("255.255.255.255"), true);
  EXPECT_EQ(UriUtils::isIPv4Address(""), false);
  EXPECT_EQ(UriUtils::isIPv4Address("..."), false);
  EXPECT_EQ(UriUtils::isIPv4Address(".0.0."), false);
  EXPECT_EQ(UriUtils::isIPv4Address(".0.0.0"), false);
  EXPECT_EQ(UriUtils::isIPv4Address("0.0.0."), false);
  EXPECT_EQ(UriUtils::isIPv4Address("00.0.0.0"), false);
  EXPECT_EQ(UriUtils::isIPv4Address("001.0.0.0"), false);
  EXPECT_EQ(UriUtils::isIPv4Address("255.255.255.256"), false);
  EXPECT_EQ(UriUtils::isIPv4Address("localhost"), false);
}

TEST(UriUtils, REMOVE_DOT_SEGMENTS) {
  // 不正なPathのときは値を比較しないので第二引数は空文字列でOK
  testRemoveDotSegments("/path/to/file", "/path/to/file", true);
  testRemoveDotSegments("/path/to/dir/", "/path/to/dir/", true);
  testRemoveDotSegments("//path//to//file", "/path/to/file", true);
  testRemoveDotSegments("//path//to//dir//", "/path/to/dir/", true);
  testRemoveDotSegments("/1/2/3/../../../4", "/4", true);
  testRemoveDotSegments("/1/2/3/4/5/../../../../../6/", "/6/", true);
  testRemoveDotSegments("", "", true);
  testRemoveDotSegments("/", "/", true);
  testRemoveDotSegments("/././/////", "/", true);
  testRemoveDotSegments("/../fail", "", false);
  testRemoveDotSegments("/1/./../../2", "", false);
}
