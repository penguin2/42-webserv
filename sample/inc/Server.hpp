#ifndef Webserv_Server_H_
#define Webserv_Server_H_
#include <string>

class Server {
 public:
  Server(const char *host, unsigned short port);
  ~Server(void);

  struct sockaddr_in customSockaddr(const char *host, unsigned short port);
  int getListenFd(void) const;
  class ServerInternalError {};

 private:
  int listen_fd_;
  Server(void);
};

#endif
