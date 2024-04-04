#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "Utils.hpp"

void testStrTrim(const char* target, const char* charset,
                 const char* expect_str) {
  std::string target_string(target);
  Utils::strTrim(target_string, charset);
  EXPECT_STREQ(target_string.c_str(), expect_str);
}

void testToLowerString(std::string str, const char* expect) {
  Utils::toLowerString(str);
  EXPECT_STREQ(str.c_str(), expect);
}

void testStrToSize_T(std::string str, int base, size_t expect_value,
                     bool expect) {
  size_t num;
  bool ret = Utils::strToSize_t(str, num, base);
  EXPECT_EQ(ret, expect);
  if (ret == true) EXPECT_EQ(num, expect_value) << str << " -> " << num;
}

void testDecodeUrlEncoding(const char* target, const char* expect_str,
                           bool expect) {
  std::string target_string(target);
  bool ret = Utils::decodeUrlEncoding(target_string);
  EXPECT_EQ(ret, expect);
  if (ret == true) EXPECT_STREQ(target_string.c_str(), expect_str);
}

TEST(Utils, STR_TRIM) {
  // target_string, charset, expect(string)
  testStrTrim("TEST1 ", " ", "TEST1");
  testStrTrim(" TEST2 ", " ", "TEST2");
  testStrTrim(" TEST3", " ", "TEST3");
  testStrTrim("		TEST4	", "	", "TEST4");
  testStrTrim("bcaTEST5abc", "abc", "TEST5");
  testStrTrim("TEST6", "EST", "6");
  testStrTrim("TEST7", "test", "TEST7");
  testStrTrim("TEST8", "T", "EST8");
  testStrTrim("TEST9T", "T", "EST9");
  testStrTrim("", "T", "");
}

TEST(Utils, IS_CONTAIN_FPTR) {
  EXPECT_EQ(Utils::isContain("SPACE UPPER", std::isdigit), false);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", std::isalpha), true);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", std::isupper), true);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", std::islower), false);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", std::isspace), true);
  EXPECT_EQ(Utils::isContain("tab	lower", std::isdigit), false);
  EXPECT_EQ(Utils::isContain("tab	lower", std::isalpha), true);
  EXPECT_EQ(Utils::isContain("tab	lower", std::isupper), false);
  EXPECT_EQ(Utils::isContain("tab	lower", std::islower), true);
  EXPECT_EQ(Utils::isContain("tab	lower", std::isspace), true);
}

TEST(Utils, IS_CONTAIN_CHARSET) {
  EXPECT_EQ(Utils::isContain("SPACE UPPER", "0123456789"), false);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", "sS"), true);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", "S"), true);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", "s"), false);
  EXPECT_EQ(Utils::isContain("SPACE UPPER", "	 "), true);
  EXPECT_EQ(Utils::isContain("tab	lower", "0123456789"), false);
  EXPECT_EQ(Utils::isContain("tab	lower", "tT"), true);
  EXPECT_EQ(Utils::isContain("tab	lower", "T"), false);
  EXPECT_EQ(Utils::isContain("tab	lower", "t"), true);
  EXPECT_EQ(Utils::isContain("tab	lower", "	"), true);
}

TEST(Utils, IS_CONTAINS_ONLY_FPTR) {
  EXPECT_EQ(Utils::isContainsOnly("TEST", std::isdigit), false);
  EXPECT_EQ(Utils::isContainsOnly("TEST", std::isalpha), true);
  EXPECT_EQ(Utils::isContainsOnly("TEST", std::isupper), true);
  EXPECT_EQ(Utils::isContainsOnly("TEST", std::islower), false);
  EXPECT_EQ(Utils::isContainsOnly("TEST", std::isspace), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", std::isdigit), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", std::isalpha), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", std::isupper), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", std::islower), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", std::isspace), true);
}

TEST(Utils, IS_CONTAINS_ONLY_CHARSET) {
  EXPECT_EQ(Utils::isContainsOnly("TEST", "1234567890"), false);
  EXPECT_EQ(Utils::isContainsOnly("TEST", "tES"), false);
  EXPECT_EQ(Utils::isContainsOnly("TEST", "TES"), true);
  EXPECT_EQ(Utils::isContainsOnly("TEST", "tes"), false);
  EXPECT_EQ(Utils::isContainsOnly("TEST", "	"), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", "1234567890"), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", "abcDEFG"), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", "ABCDEFG"), false);
  EXPECT_EQ(Utils::isContainsOnly("	   ", "abcdefg"), false);
  EXPECT_EQ(Utils::isContainsOnly("    ", " "), true);
}

TEST(Utils, TO_LOWER_STRING) {
  // string, expect(string)
  testToLowerString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz");
  testToLowerString("1234567890", "1234567890");
  testToLowerString("aBcDeFg", "abcdefg");
  testToLowerString(" TEST ", " test ");
  testToLowerString("", "");
}

TEST(Utils, STR_TO_SIZE_T_SUCCESS) {
  // string, base(8 or 10 or 16), expect_value(size_t), expect(bool)
  testStrToSize_T("0", 10, 0, true);
  testStrToSize_T("1", 8, 1, true);
  testStrToSize_T("1", 10, 1, true);
  testStrToSize_T("1", 16, 1, true);
  testStrToSize_T("00000000000000000000001", 8, 1, true);
  testStrToSize_T("00000000000000000000001", 10, 1, true);
  testStrToSize_T("00000000000000000000001", 16, 1, true);
  testStrToSize_T("77", 8, (7 * 8) + 7, true);
  testStrToSize_T("99", 10, (9 * 10) + 9, true);
  testStrToSize_T("FF", 16, (15 * 16) + 15, true);
  testStrToSize_T("fF", 16, (15 * 16) + 15, true);
  testStrToSize_T("ff", 16, (15 * 16) + 15, true);
  testStrToSize_T("18446744073709551615", 10, SIZE_MAX, true);
}

TEST(Utils, STR_TO_SIZE_T_ERROR) {
  // if (expect(bool) == false): expect_value(size_t) = -1;
  testStrToSize_T("", 10, -1, false);
  testStrToSize_T("1", -1, -1, false);
  testStrToSize_T("1", 32, -1, false);
  testStrToSize_T(" 1", 10, -1, false);
  testStrToSize_T("1 ", 10, -1, false);
  testStrToSize_T("a1", 10, -1, false);
  testStrToSize_T("1a", 10, -1, false);
  testStrToSize_T("-1", 8, -1, false);
  testStrToSize_T("-1", 10, -1, false);
  testStrToSize_T("-1", 16, -1, false);
  testStrToSize_T("+1", 8, -1, false);
  testStrToSize_T("+1", 10, -1, false);
  testStrToSize_T("+1", 16, -1, false);
  testStrToSize_T("18446744073709551616", 10, -1, false);  // SIZE_MAX + 1
}

TEST(Utils, DECODE_URL_ENCODING_SUCCESS) {
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

TEST(Utils, DECODE_URL_ENCODING_ERROR) {
  // if (expect(bool) == false): expect(string) = -1;
  testDecodeUrlEncoding("%2", "-1", false);
  testDecodeUrlEncoding("%2%20", "-1", false);
  testDecodeUrlEncoding("%ZX%20", "-1", false);
  testDecodeUrlEncoding("%zx%20", "-1", false);
}
