#ifndef WEBSERV_SYS_UTILS_H_
#define WEBSERV_SYS_UTILS_H_

#include <map>
#include <string>

namespace sys_utils {
int addNonblockingFlag(int fd);
int addCloseOnExecFlag(int fd);
int makeListenSocket(const char* hostname, const char* servname, int backlog);
char* convertToCstring(const std::string& str);
void deleteCstring(char* cstr);
char* const* convertToEnvp(const std::map<std::string, std::string> envp_map);
void deleteCstringArray(char* const* c_str_array);
int clearFd(int* fd);
pid_t waitNoHang(pid_t pid, int* status);
int killAndWaitProcess(pid_t pid);
}  // namespace sys_utils

#endif
