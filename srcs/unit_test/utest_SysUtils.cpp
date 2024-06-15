#include <gtest/gtest.h>

#include <cstring>

#include "utils/SysUtils.hpp"

static void testConvertToCstring(const std::string& src) {
  char* dst = sys_utils::convertToCstring(src);
  EXPECT_TRUE(std::strcmp(dst, src.c_str()) == 0);
  sys_utils::deleteCstring(dst);
}

TEST(SysUtils, convertToCstring) {
  testConvertToCstring("");
  testConvertToCstring("hello_world!");
  testConvertToCstring("42");
}

TEST(SysUtils, convertToEnvp) {
  std::map<std::string, std::string> test_map;
  test_map[""] = "with_empty_key";
  test_map["with_empty_value"] = "";
  test_map["key"] = "value";
  test_map["hello"] = "world";

  char* const* envp = sys_utils::convertToEnvp(test_map);
  EXPECT_EQ(std::string(envp[0]), "=with_empty_key");
  EXPECT_EQ(std::string(envp[1]), "hello=world");
  EXPECT_EQ(std::string(envp[2]), "key=value");
  EXPECT_EQ(std::string(envp[3]), "with_empty_value=");
  EXPECT_EQ(envp[4], nullptr);

  sys_utils::deleteCstringArray(envp);
}

TEST(SysUtils, convertToEnvp_with_null_map) {
  std::map<std::string, std::string> null_map;

  char* const* envp = sys_utils::convertToEnvp(null_map);
  EXPECT_EQ(envp[0], nullptr);

  sys_utils::deleteCstringArray(envp);
}
