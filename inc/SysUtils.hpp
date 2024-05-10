#ifndef WEBSERV_SYS_UTILS_H_
#define WEBSERV_SYS_UTILS_H_

#include <map>
#include <string>

class SysUtils {
 public:
  static const int kDefaultListenBacklog = 511;

  static int addNonblockingFlag(int fd);
  static int addCloseOnExecFlag(int fd);
  static int makeListenSocket(const char* hostname, const char* servname,
                              int backlog);
  static char* convertToCstring(const std::string& str);
  static void deleteCstring(char* cstr);
  static char* const* convertToEnvp(
      const std::map<std::string, std::string> envp_map);
  static void deleteCstringArray(char* const* c_str_array);
  static int clearFd(int* fd);

 private:
  SysUtils(void);
  ~SysUtils(void);
  SysUtils(const SysUtils&);
  void operator=(const SysUtils&);
};

#endif
