#include "SysUtils.hpp"

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstring>

#include "Logger.hpp"

int SysUtils::addNonblockingFlag(int fd) {
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
    LOG(WARN, "fcntl(F_SETFL): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int SysUtils::addCloseOnExecFlag(int fd) {
  if (fcntl(fd, F_SETFD, FD_CLOEXEC) < 0) {
    LOG(WARN, "fcntl(F_SETFD): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int SysUtils::makeListenSocket(const char* host, const char* service,
                               int backlog) {
  int socket_fd;
  struct addrinfo hints, *listp, *p;

  std::memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  const int status = getaddrinfo(host, service, &hints, &listp);
  if (status != 0) {
    LOG(WARN, "getaddrinfo: ", gai_strerror(status));
    return -1;
  }

  const int enable = 1;
  for (p = listp; p != NULL; p = p->ai_next) {
    socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_fd < 0) continue;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
        0) {
      LOG(WARN, "setsockopt: ", std::strerror(errno));
    }
    if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == 0) break;
    if (close(socket_fd) < 0) {
      LOG(WARN, "close: ", std::strerror(errno));
      freeaddrinfo(listp);
      return -1;
    }
  }
  freeaddrinfo(listp);

  if (p == NULL) {
    LOG(WARN, "makeListenSocket failed on: ",
        std::string(host) + ":" + std::string(service));
    return -1;
  }

  if (SysUtils::addNonblockingFlag(socket_fd) < 0 ||
      SysUtils::addCloseOnExecFlag(socket_fd) < 0) {
    LOG(WARN,
        "makeListenSocket failed on: ", "add nonblocking/closeonexec flags");
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, backlog) < 0) {
    LOG(WARN, "listen: ", std::strerror(errno));
    close(socket_fd);
    return -1;
  }

  return socket_fd;
}

char* SysUtils::convertToCstring(const std::string& str) {
  char* cstr = new char[str.size() + 1];
  std::strcpy(cstr, str.c_str());
  return cstr;
}

void SysUtils::deleteCstring(char* cstr) { delete cstr; }

char* const* SysUtils::convertToEnvp(
    const std::map<std::string, std::string> envp_map) {
  const std::size_t envp_size = envp_map.size();
  char** envp = new char*[envp_size + 1];

  std::size_t cnt_idx = 0;
  for (std::map<std::string, std::string>::const_iterator it = envp_map.begin();
       it != envp_map.end(); ++it) {
    const std::string key_value_str = it->first + "=" + it->second;
    char* key_value_c_str = new char[key_value_str.size() + 1];
    envp[cnt_idx] = std::strcpy(key_value_c_str, key_value_str.c_str());
    ++cnt_idx;
  }
  envp[cnt_idx] = NULL;
  return envp;
}

void SysUtils::deleteCstringArray(char* const* c_str_array) {
  for (std::size_t idx = 0; c_str_array[idx] != NULL; ++idx)
    delete[] c_str_array[idx];
  delete[] c_str_array;
}

int SysUtils::clearFd(int* fd) {
  if (*fd != -1) {
    if (close(*fd) < 0) return -1;
    *fd = -1;
  }
  return 0;
}

pid_t SysUtils::waitNoHang(pid_t pid, int* status) {
  if (pid == -1) return -1;

  int stat_loc;
  const pid_t exited_pid = waitpid(pid, &stat_loc, WNOHANG);
  if (exited_pid < 0)
    LOG(WARN, "waitpid: ", std::strerror(errno));
  else if (exited_pid > 0 && status != NULL) {
    if (WIFEXITED(stat_loc))
      *status = WEXITSTATUS(stat_loc);
    else if (WIFSIGNALED(stat_loc))
      *status = WTERMSIG(stat_loc) + 128;
  }
  return exited_pid;
}

int SysUtils::killAndWaitProcess(pid_t pid) {
  if (kill(pid, SIGTERM) < 0 || kill(pid, SIGKILL) < 0) {
    LOG(WARN, "kill(cgi): ", std::strerror(errno));
    return -1;
  }
  if (waitpid(pid, NULL, 0) != pid) {
    LOG(WARN, "waitpid(cgi): after kill: ", std::strerror(errno));
    return -1;
  }
  return 0;
}
