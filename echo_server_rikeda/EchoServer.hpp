#ifndef Webserv_EchoServer_H_
#define Webserv_EchoServer_H_
#include <string>

class EchoServer {
 public:
  EchoServer(const char *host, unsigned short port);
  ~EchoServer(void);

  struct sockaddr_in customSockaddr(const char *host, unsigned short port);
  int getListenFd(void) const;
  class ServerInternalError {};

 private:
  int listen_fd_;
  EchoServer(void);
};

#endif
