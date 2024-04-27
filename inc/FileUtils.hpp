#ifndef WEBSERV_FILEUTILS_H_
#define WEBSERV_FILEUTILS_H_

#include <string>

class FileUtils {
 public:
  static bool isExistDir(const std::string& file_path);
  static bool isExistFile(const std::string& file_path);
  static bool hasFilePermission(const std::string& file_path, int type);
  static bool writeAllDataToFile(const std::string& file_path,
                                 const std::string& data);
  static bool readAllDataFromFile(const std::string& file_path,
                                  std::stringstream& ss);

 private:
  FileUtils(void);
  ~FileUtils(void);
  FileUtils(const FileUtils&);
  const FileUtils operator=(const FileUtils&);
};

#endif
