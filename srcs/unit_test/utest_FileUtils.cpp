#include <gtest/gtest.h>
#include <unistd.h>

#include <sstream>

#include "FileUtils.hpp"

void testReadAllDataFromFile(const std::string& file, bool expect) {
  std::stringstream ss;
  EXPECT_EQ(FileUtils::readAllDataFromFile(file, ss), expect);
}

TEST(FileUtils, IS_EXIST_DIR) {
  EXPECT_EQ(FileUtils::isExistDir("/"), true);
  EXPECT_EQ(FileUtils::isExistDir("./srcs/"), true);
  EXPECT_EQ(FileUtils::isExistDir("./inc/"), true);
  EXPECT_EQ(FileUtils::isExistDir("../../no_exist/"), false);
  EXPECT_EQ(FileUtils::isExistDir("./Makefile"), false);
  EXPECT_EQ(FileUtils::isExistDir("./srcs/main.cpp"), false);
}

TEST(FileUtils, IS_EXIST_FILE) {
  EXPECT_EQ(FileUtils::isExistFile("./Makefile"), true);
  EXPECT_EQ(FileUtils::isExistFile("./srcs/main.cpp"), true);
  EXPECT_EQ(FileUtils::isExistFile("/"), false);
  EXPECT_EQ(FileUtils::isExistFile("./inc/"), false);
  EXPECT_EQ(FileUtils::isExistFile("./srcs/"), false);
}

TEST(FileUtils, HAS_FILE_PERMISSION) {
  EXPECT_EQ(FileUtils::hasFilePermission("./Makefile", W_OK), true);
  EXPECT_EQ(FileUtils::hasFilePermission("./Makefile", R_OK), true);
  EXPECT_EQ(FileUtils::hasFilePermission("./srcs/", W_OK), true);
  EXPECT_EQ(FileUtils::hasFilePermission("./srcs/", R_OK), true);
  EXPECT_EQ(FileUtils::hasFilePermission("/no_exist", W_OK), false);
  EXPECT_EQ(FileUtils::hasFilePermission("/no_exist", R_OK), false);
  EXPECT_EQ(FileUtils::hasFilePermission("/", W_OK), false);
  EXPECT_EQ(FileUtils::hasFilePermission("/", R_OK), true);
}

TEST(FileUtils, READ_ALL_DATA_FROM_FILE) {
  // file, directoryの判定や権限確認は対象外、ファイルが存在すれば成功
  testReadAllDataFromFile("/bin/ls", true);
  testReadAllDataFromFile("./Makefile", true);
  testReadAllDataFromFile("./inc/", true);
  testReadAllDataFromFile("./inc", true);
  testReadAllDataFromFile("", false);
  testReadAllDataFromFile("./nofile", false);
  testReadAllDataFromFile("./nofile.html", false);
}
