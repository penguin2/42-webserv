#ifndef WEBSERV_FILEUTILS_H_
#define WEBSERV_FILEUTILS_H_

#include <dirent.h>
#include <sys/types.h>

#include <string>
#include <vector>

namespace file_utils {
bool isExistDir(const std::string& file_path);
bool isExistFile(const std::string& file_path);
bool hasFilePermission(const std::string& file_path, int type);
bool writeAllDataToFile(const std::string& file_path, const std::string& data);
bool readAllDataFromFile(const std::string& file_path, std::stringstream& ss);
off_t getFileSize(const std::string& file_path);

class Entry {
 public:
  enum Type { DIRECTORY = 0, REGFILE = 1, UNKNOWN = 2 };

  Entry(const struct dirent& dir_ent);
  ~Entry(void);
  Entry(const Entry&);
  Entry& operator=(const Entry&);
  bool operator>(const Entry& entry) const;
  bool operator<(const Entry& entry) const;
  bool operator>=(const Entry& entry) const;
  bool operator<=(const Entry& entry) const;
  bool operator==(const Entry& entry) const;
  bool operator!=(const Entry& entry) const;

  static std::vector<Entry> readDirData(const std::string& dir_path);
  const std::string& getFileName(void) const;
  Type getFileType(void) const;
  bool startWithDot(void) const;

 private:
  std::string file_name_;
  Type type_;

  Entry(void);
};
}  // namespace file_utils

#endif
