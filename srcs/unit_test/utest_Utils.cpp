#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "utils/Utils.hpp"

void testStrTrim(const char* target, const char* charset,
                 const char* expect_str) {
  std::string target_string(target);
  utils::strTrim(target_string, charset);
  EXPECT_STREQ(target_string.c_str(), expect_str);
}

void testStrLTrim(const char* target, const char* charset,
                  const char* expect_str) {
  std::string target_string(target);
  utils::strLTrim(target_string, charset);
  EXPECT_STREQ(target_string.c_str(), expect_str);
}

void testToLowerString(std::string str, const char* expect) {
  utils::toLowerString(str);
  EXPECT_STREQ(str.c_str(), expect);
}

void testToUpperString(std::string str, const char* expect) {
  utils::toUpperString(str);
  EXPECT_STREQ(str.c_str(), expect);
}

void testStrToSize_T(std::string str, int base, size_t expect_value,
                     bool expect) {
  size_t num;
  bool ret = utils::strToSize_t(str, num, base);
  EXPECT_EQ(ret, expect);
  if (ret == true) EXPECT_EQ(num, expect_value) << str << " -> " << num;
}

void testSplit(std::string str, char separator,
               std::vector<std::string> expect_vector) {
  std::vector<std::string> vec = utils::split(str, separator);
  ASSERT_EQ(vec.size(), expect_vector.size());
  for (size_t i = vec.size(); i < vec.size(); ++i) {
    ASSERT_STREQ(vec[i].c_str(), expect_vector[i].c_str());
  }
}

void testJoinStrings(std::vector<std::string> vec, std::string delim,
                     const char* expect_str) {
  EXPECT_STREQ(utils::joinStrings(vec, delim).c_str(), expect_str);
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

TEST(Utils, STR_LTRIM) {
  // target_string, charset, expect(string)
  testStrLTrim("TEST1 ", " ", "TEST1 ");
  testStrLTrim(" TEST2 ", " ", "TEST2 ");
  testStrLTrim(" TEST3", " ", "TEST3");
  testStrLTrim("		TEST4	", "	", "TEST4	");
  testStrLTrim("bcaTEST5abc", "abc", "TEST5abc");
  testStrLTrim("TEST6", "EST", "6");
  testStrLTrim("TEST7", "test", "TEST7");
  testStrLTrim("TEST8", "T", "EST8");
  testStrLTrim("TEST9T", "T", "EST9T");
  testStrLTrim("", "T", "");
}

TEST(Utils, IS_CONTAIN_FPTR) {
  EXPECT_EQ(utils::isContain("SPACE UPPER", std::isdigit), false);
  EXPECT_EQ(utils::isContain("SPACE UPPER", std::isalpha), true);
  EXPECT_EQ(utils::isContain("SPACE UPPER", std::isupper), true);
  EXPECT_EQ(utils::isContain("SPACE UPPER", std::islower), false);
  EXPECT_EQ(utils::isContain("SPACE UPPER", std::isspace), true);
  EXPECT_EQ(utils::isContain("tab	lower", std::isdigit), false);
  EXPECT_EQ(utils::isContain("tab	lower", std::isalpha), true);
  EXPECT_EQ(utils::isContain("tab	lower", std::isupper), false);
  EXPECT_EQ(utils::isContain("tab	lower", std::islower), true);
  EXPECT_EQ(utils::isContain("tab	lower", std::isspace), true);
}

TEST(Utils, IS_CONTAIN_CHARSET) {
  EXPECT_EQ(utils::isContain("SPACE UPPER", "0123456789"), false);
  EXPECT_EQ(utils::isContain("SPACE UPPER", "sS"), true);
  EXPECT_EQ(utils::isContain("SPACE UPPER", "S"), true);
  EXPECT_EQ(utils::isContain("SPACE UPPER", "s"), false);
  EXPECT_EQ(utils::isContain("SPACE UPPER", "	 "), true);
  EXPECT_EQ(utils::isContain("tab	lower", "0123456789"), false);
  EXPECT_EQ(utils::isContain("tab	lower", "tT"), true);
  EXPECT_EQ(utils::isContain("tab	lower", "T"), false);
  EXPECT_EQ(utils::isContain("tab	lower", "t"), true);
  EXPECT_EQ(utils::isContain("tab	lower", "	"), true);
}

TEST(Utils, IS_CONTAINS_ONLY_FPTR) {
  EXPECT_EQ(utils::isContainsOnly("TEST", std::isdigit), false);
  EXPECT_EQ(utils::isContainsOnly("TEST", std::isalpha), true);
  EXPECT_EQ(utils::isContainsOnly("TEST", std::isupper), true);
  EXPECT_EQ(utils::isContainsOnly("TEST", std::islower), false);
  EXPECT_EQ(utils::isContainsOnly("TEST", std::isspace), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", std::isdigit), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", std::isalpha), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", std::isupper), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", std::islower), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", std::isspace), true);
}

TEST(Utils, IS_CONTAINS_ONLY_CHARSET) {
  EXPECT_EQ(utils::isContainsOnly("TEST", "1234567890"), false);
  EXPECT_EQ(utils::isContainsOnly("TEST", "tES"), false);
  EXPECT_EQ(utils::isContainsOnly("TEST", "TES"), true);
  EXPECT_EQ(utils::isContainsOnly("TEST", "tes"), false);
  EXPECT_EQ(utils::isContainsOnly("TEST", "	"), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", "1234567890"), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", "abcDEFG"), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", "ABCDEFG"), false);
  EXPECT_EQ(utils::isContainsOnly("	   ", "abcdefg"), false);
  EXPECT_EQ(utils::isContainsOnly("    ", " "), true);
}

TEST(Utils, TO_LOWER_STRING) {
  // string, expect(string)
  testToLowerString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz");
  testToLowerString("1234567890", "1234567890");
  testToLowerString("aBcDeFg", "abcdefg");
  testToLowerString(" TEST ", " test ");
  testToLowerString("", "");
}

TEST(Utils, TO_LOWER) {
  EXPECT_EQ(utils::toLower("ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
            "abcdefghijklmnopqrstuvwxyz");
  EXPECT_EQ(utils::toLower("1234567890"), "1234567890");
  EXPECT_EQ(utils::toLower("aBcDeFg"), "abcdefg");
  EXPECT_EQ(utils::toLower(" TEST "), " test ");
  EXPECT_EQ(utils::toLower(""), "");
}

TEST(Utils, TO_UPPER_STRING) {
  // string, expect(string)
  testToUpperString("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  testToUpperString("1234567890", "1234567890");
  testToUpperString("aBcDeFg", "ABCDEFG");
  testToUpperString(" test ", " TEST ");
  testToUpperString("", "");
}

TEST(Utils, TO_UPPER) {
  EXPECT_EQ(utils::toUpper("abcdefghijklmnopqrstuvwxyz"),
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  EXPECT_EQ(utils::toUpper("1234567890"), "1234567890");
  EXPECT_EQ(utils::toUpper("aBcDeFg"), "ABCDEFG");
  EXPECT_EQ(utils::toUpper(" test "), " TEST ");
  EXPECT_EQ(utils::toUpper(""), "");
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

TEST(Utils, isStartsWith_TRUE) {
  EXPECT_TRUE(utils::isStartsWith("", ""));
  EXPECT_TRUE(utils::isStartsWith("hello", "hello"));
  EXPECT_TRUE(utils::isStartsWith("banana", ""));
  EXPECT_TRUE(utils::isStartsWith("banana", "b"));
  EXPECT_TRUE(utils::isStartsWith("banana", "ban"));
  EXPECT_TRUE(utils::isStartsWith("banana", "banana"));
}

TEST(Utils, isStartsWith_FALSE) {
  EXPECT_FALSE(utils::isStartsWith("", "42"));
  EXPECT_FALSE(utils::isStartsWith("hello", "world"));
  EXPECT_FALSE(utils::isStartsWith("banana", "false"));
  EXPECT_FALSE(utils::isStartsWith("banana", "banana-banana"));
  EXPECT_FALSE(utils::isStartsWith("banana-apple", "banana-banana"));
}

TEST(Utils, IS_ENDS_WITH) {
  EXPECT_TRUE(utils::isEndsWith("", ""));
  EXPECT_TRUE(utils::isEndsWith("abc", "abc"));
  EXPECT_TRUE(utils::isEndsWith("abc", "bc"));
  EXPECT_TRUE(utils::isEndsWith("abc", "c"));
  EXPECT_TRUE(utils::isEndsWith("abc", ""));

  EXPECT_FALSE(utils::isEndsWith("", "abc"));
  EXPECT_FALSE(utils::isEndsWith("def", "abc"));
  EXPECT_FALSE(utils::isEndsWith("def", "abcdef"));
  EXPECT_FALSE(utils::isEndsWith("123-456-789", "-456"));
}

TEST(Utils, isStartsWithCaseInsensitive_TRUE) {
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("", ""));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("hello", "hello"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", ""));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "b"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "ban"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "banana"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("", ""));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("hello", "HeLlo"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", ""));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "B"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "bAN"));
  EXPECT_TRUE(utils::isStartsWithCaseInsensitive("banana", "BaNaNa"));
}

TEST(Utils, isStartsWithCaseInsensitive_FALSE) {
  EXPECT_FALSE(utils::isStartsWithCaseInsensitive("", "42"));
  EXPECT_FALSE(utils::isStartsWithCaseInsensitive("hello", "world"));
  EXPECT_FALSE(utils::isStartsWithCaseInsensitive("banana", "false"));
  EXPECT_FALSE(utils::isStartsWithCaseInsensitive("banana", "banana-banana"));
  EXPECT_FALSE(
      utils::isStartsWithCaseInsensitive("banana-apple", "banana-banana"));
  EXPECT_FALSE(utils::isStartsWithCaseInsensitive("banana", "BANANA-BANANA"));
  EXPECT_FALSE(
      utils::isStartsWithCaseInsensitive("banana-apple", "BANANA-BANANA"));
}

TEST(Utils, popFrontSubstr) {
  std::string test_str = "hello_world";
  EXPECT_EQ(utils::popFrontSubstr(test_str, 0), "");
  EXPECT_EQ(test_str, "hello_world");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 1), "h");
  EXPECT_EQ(test_str, "ello_world");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 3), "ell");
  EXPECT_EQ(test_str, "o_world");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 5), "o_wor");
  EXPECT_EQ(test_str, "ld");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 10000), "ld");
  EXPECT_EQ(test_str, "");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 10000), "");
  EXPECT_EQ(test_str, "");
  EXPECT_EQ(utils::popFrontSubstr(test_str, 10000), "");
  EXPECT_EQ(test_str, "");
}

TEST(Utils, GET_EXTENSION) {
  EXPECT_STREQ(utils::getExtension("index.html").c_str(), "html");
  EXPECT_STREQ(utils::getExtension("../../index.html").c_str(), "html");
  EXPECT_STREQ(utils::getExtension("./index.html").c_str(), "html");
  EXPECT_STREQ(utils::getExtension("index.").c_str(), "");
  EXPECT_STREQ(utils::getExtension("").c_str(), "");
  EXPECT_STREQ(utils::getExtension("logo.png").c_str(), "png");
  EXPECT_STREQ(utils::getExtension("main.cpp").c_str(), "cpp");
  EXPECT_STREQ(utils::getExtension("/bin/ls").c_str(), "");
  EXPECT_STREQ(utils::getExtension("//./bin/ls").c_str(), "");
  EXPECT_STREQ(utils::getExtension("www.google.com").c_str(), "com");
  EXPECT_STREQ(utils::getExtension("/0.0.0.0/a.py?f=a.zip").c_str(), "zip");
  EXPECT_STREQ(utils::getExtension("......").c_str(), "");
  EXPECT_STREQ(utils::getExtension("../").c_str(), "");
}

TEST(Utils, SPLIT) {
  testSplit("/a/b/c/", '/', {"a", "b", "c"});
  testSplit("///a/b/c///", '/', {"a", "b", "c"});
  testSplit("/api/form/./../../admin", '/',
            {"api", "form", ".", "..", "..", "admin"});
  testSplit("a/b/c/", '/', {"a", "b", "c"});
  testSplit("a/b/c", '/', {"a", "b", "c"});
  testSplit("a/b/c", '=', {"abc"});
  testSplit("", '=', {});
  testSplit("", '\0', {});
}

TEST(Utils, JOIN_STRINGS) {
  testJoinStrings({"GET", "POST", "DELETE"}, ", ", "GET, POST, DELETE");
  testJoinStrings({"GET", "POST", "DELETE"}, "", "GETPOSTDELETE");
  testJoinStrings({"PATH", "TO", "FILE"}, "/", "PATH/TO/FILE");
  testJoinStrings({"ONE", "TWO"}, "&&", "ONE&&TWO");
  testJoinStrings({"WORD"}, "/", "WORD");
  testJoinStrings({}, "/", "");
  testJoinStrings({"a", "b", "c"}, " -> ", "a -> b -> c");
}

TEST(Utils, IS_SAME_VALUE_CASE_INSENSITIVE) {
  std::map<std::string, std::string> test_mp;
  test_mp["a"] = "fourty-two";
  test_mp["A"] = "ft-42";
  test_mp["b"] = "aPpLe";

  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "a", "fourty-two"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "a", "FOURTY-TWO"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "a", "FouRty-tWo"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "A", "ft-42"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "A", "FT-42"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "b", "APPLE"));
  EXPECT_TRUE(utils::isSameValueCaseInsensitive(test_mp, "b", "apple"));

  EXPECT_FALSE(utils::isSameValueCaseInsensitive(test_mp, "a", "fourtytwo"));
  EXPECT_FALSE(
      utils::isSameValueCaseInsensitive(test_mp, "a", "fourty-two-fourty-two"));
  EXPECT_FALSE(utils::isSameValueCaseInsensitive(test_mp, "A", "ft_42"));
  EXPECT_FALSE(utils::isSameValueCaseInsensitive(test_mp, "b", "app"));
  EXPECT_FALSE(utils::isSameValueCaseInsensitive(test_mp, "B", "apple"));
}

TEST(Utils, UINT_TO_STRING) {
  EXPECT_EQ(utils::uintToString(0U), "0");
  EXPECT_EQ(utils::uintToString(1U), "1");
  EXPECT_EQ(utils::uintToString(10U), "10");
  EXPECT_EQ(utils::uintToString(0x7fU), "127");
  EXPECT_EQ(utils::uintToString(0xffU), "255");
  EXPECT_EQ(utils::uintToString(12345U), "12345");
  EXPECT_EQ(utils::uintToString(65535U), "65535");
}

TEST(Utils, concatWithSlash) {
  EXPECT_STREQ(utils::concatWithSlash("abc", "def").c_str(), "abc/def");
  EXPECT_STREQ(utils::concatWithSlash("abc/", "def").c_str(), "abc/def");
  EXPECT_STREQ(utils::concatWithSlash("abc", "/def").c_str(), "abc/def");
  EXPECT_STREQ(utils::concatWithSlash("abc/", "/").c_str(), "abc/");
  EXPECT_STREQ(utils::concatWithSlash("abc", "/").c_str(), "abc/");
  EXPECT_STREQ(utils::concatWithSlash("abc", "").c_str(), "abc/");
  EXPECT_STREQ(utils::concatWithSlash("/", "/def").c_str(), "/def");
  EXPECT_STREQ(utils::concatWithSlash("/", "def").c_str(), "/def");
  EXPECT_STREQ(utils::concatWithSlash("", "def").c_str(), "/def");
  EXPECT_STREQ(utils::concatWithSlash("", "").c_str(), "/");
}
