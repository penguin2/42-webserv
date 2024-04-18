#ifndef WEBSERV_SYS_UTILS_H_
#define WEBSERV_SYS_UTILS_H_

#include <map>
#include <string>

class SysUtils {
 public:
  static int addNonblockingFlag(int fd);
  static int makeListenSocket(const std::string& port, int backlog);
  static char* const* convertToEnvp(
      const std::map<std::string, std::string> envp_map);
  static void deleteCstringArray(char* const* c_str_array);

 private:
  SysUtils(void);
  ~SysUtils(void);
  SysUtils(const SysUtils&);
  void operator=(const SysUtils&);
};

#endif
