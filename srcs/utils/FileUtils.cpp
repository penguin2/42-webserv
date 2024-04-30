#include "FileUtils.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool FileUtils::isExistDir(const std::string& file_path) {
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  if (S_ISDIR(st.st_mode)) return true;
  return false;
}

bool FileUtils::isExistFile(const std::string& file_path) {
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  if (S_ISREG(st.st_mode)) return true;
  return false;
}

bool FileUtils::hasFilePermission(const std::string& file_path, int type) {
  if (access(file_path.c_str(), type) == 0) return true;
  return false;
}

bool FileUtils::writeAllDataToFile(const std::string& file_path,
                                   const std::string& data) {
  std::ofstream ofs(file_path.c_str());

  if (ofs.fail()) return false;
  ofs << data;
  if (ofs.fail()) return false;
  return true;
}

// file or dir の確認や権限確認はこの関数外で行う
bool FileUtils::readAllDataFromFile(const std::string& file,
                                    std::stringstream& ss) {
  std::ifstream ifs(file.c_str(), std::ios::binary);

  if (ifs.fail()) return false;
  ss << ifs.rdbuf();
  return true;
}

off_t FileUtils::getFileSize(const std::string& file_path) {
  struct stat file_st;

  if (stat(file_path.c_str(), &file_st) == 0) return file_st.st_size;
  return -1;
}

FileUtils::Entry::Entry(const struct dirent& dir_ent)
    : file_name_(dir_ent.d_name) {
  switch (dir_ent.d_type) {
    case (DT_DIR):
      this->type_ = FileUtils::Entry::DIRECTORY;
      break;
    case (DT_REG):
      this->type_ = FileUtils::Entry::REGFILE;
      break;
    default:
      this->type_ = FileUtils::Entry::UNKNOWN;
  }
}

FileUtils::Entry::~Entry(void) {}

FileUtils::Entry::Entry(const Entry& entry) {
  this->file_name_ = entry.file_name_;
  this->type_ = entry.type_;
}

FileUtils::Entry& FileUtils::Entry::operator=(const Entry& entry) {
  if (this != &entry) {
    this->file_name_ = entry.file_name_;
    this->type_ = entry.type_;
  }
  return *this;
}

bool FileUtils::Entry::operator>(const Entry& entry) const {
  if (this->type_ != entry.type_) return (this->type_ > entry.type_);
  return (this->file_name_ > entry.file_name_);
}

bool FileUtils::Entry::operator<(const Entry& entry) const {
  if (this->type_ != entry.type_) return (this->type_ < entry.type_);
  return (this->file_name_ < entry.file_name_);
}

bool FileUtils::Entry::operator>=(const Entry& entry) const {
  return ((*this == entry) || (*this > entry));
}

bool FileUtils::Entry::operator<=(const Entry& entry) const {
  return ((*this == entry) || (*this < entry));
}

bool FileUtils::Entry::operator==(const Entry& entry) const {
  return ((this->type_ == entry.type_) &&
          (this->file_name_ == entry.file_name_));
}

bool FileUtils::Entry::operator!=(const Entry& entry) const {
  return (!(*this == entry));
}

// 存在しないディレクトリなら空のvector
// 空のディレクトリならカレントディレクトリと親ディレクトリのパスのvector
std::vector<FileUtils::Entry> FileUtils::Entry::readDirData(
    const std::string& dir_path) {
  std::vector<FileUtils::Entry> dir_data;
  DIR* dir = opendir(dir_path.c_str());

  if (dir == NULL) return dir_data;
  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    dir_data.push_back(*entry);
  }
  closedir(dir);
  return dir_data;
}

const std::string& FileUtils::Entry::getFileName(void) const {
  return this->file_name_;
}

FileUtils::Entry::Type FileUtils::Entry::getFileType(void) const {
  return this->type_;
}

bool FileUtils::Entry::startWithDot(void) const {
  return (this->file_name_[0] == '.');
}
