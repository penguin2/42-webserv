#include "FileUtils.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool FileUtils::isExistDir(const std::string &file_path) {
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  if ((st.st_mode & S_IFMT) == S_IFDIR) return true;
  return false;
}

bool FileUtils::isExistFile(const std::string &file_path) {
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  if ((st.st_mode & S_IFMT) == S_IFREG) return true;
  return false;
}

bool FileUtils::hasFilePermission(const std::string &file_path, int type) {
  if (access(file_path.c_str(), type) == 0) return true;
  return false;
}

bool FileUtils::writeAllDataToFile(const std::string &file_path,
                                   const std::string &data) {
  std::ofstream ofs(file_path.c_str());

  if (!ofs.is_open()) return false;
  ofs << data;
  ofs.close();
  return true;
}

// ファイルからデータを読み取り文字列に変換
// file or dir の確認や権限確認はこの関数外で行う想定
// if return is false -> ServerException(500)
bool FileUtils::readAllDataFromFile(const std::string &file,
                                    std::stringstream &ss) {
  std::ifstream ifs(file.c_str(), std::ios::binary);

  if (ifs.fail()) return false;
  ss << ifs.rdbuf();
  ifs.close();
  if (ifs.eof() == false && ifs.fail()) return false;
  return true;
}

// 存在しないディレクトリなら空のvector
// 空のディレクトリならカレントディレクトリと親ディレクトリのパスのvector
std::vector<struct dirent> FileUtils::readDirData(const std::string &dir_path) {
  std::vector<struct dirent> dir_data;
  DIR *dir = opendir(dir_path.c_str());

  if (dir == NULL) return dir_data;
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    dir_data.push_back(*entry);
  }
  closedir(dir);
  return dir_data;
}

off_t FileUtils::getFileSize(const std::string &file_path) {
  struct stat file_st;

  if (stat(file_path.c_str(), &file_st) == 0) return file_st.st_size;
  return -1;
}
