#ifndef Webserv_AHandler_H_
#define Webserv_AHandler_H_

#include <exception>

#include "Server.hpp"

#define PROXY_HOST "127.0.0.1"
#define SERVER_HOST "127.0.0.1"
#define PROXY_PORT 4242
#define SERVER_PORT 4243

class AHandler {
 public:
  AHandler(size_t max_event_size);
  virtual ~AHandler(void);

  class HandlerError : std::exception {};
  virtual void startUpHandle(Server server) = 0;

 protected:
  void add(int fd, int event);
  void del(int fd);
  int epoll_fd_;
  struct epoll_event* ev_list_;
  size_t max_event_size_;

 private:
  struct epoll_event customEpollEvent(int fd, int event);
};

#endif
