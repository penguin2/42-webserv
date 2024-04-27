#ifndef WEBSERV_FILEUTILS_H_
#define WEBSERV_FILEUTILS_H_

#include <sys/types.h>

#include <string>
#include <vector>

class FileUtils {
 public:
  static bool isExistDir(const std::string& file_path);
  static bool isExistFile(const std::string& file_path);
  static bool hasFilePermission(const std::string& file_path, int type);
  static bool writeAllDataToFile(const std::string& file_path,
                                 const std::string& data);
  static bool readAllDataFromFile(const std::string& file_path,
                                  std::stringstream& ss);
  static std::vector<struct dirent> readDirData(const std::string& dir_path);
  static off_t getFileSize(const std::string& file_path);

 private:
  FileUtils(void);
  ~FileUtils(void);
  FileUtils(const FileUtils&);
  const FileUtils operator=(const FileUtils&);
};

#endif
