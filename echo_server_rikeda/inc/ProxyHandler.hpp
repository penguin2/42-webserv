#ifndef Wevserv_ProxyHandler_H_
#define Wevserv_ProxyHandler_H_
#include <exception>
#include <string>

#include "Server.hpp"

class ProxyHandler {
 public:
  ProxyHandler(size_t max_event_size);
  ~ProxyHandler(void);

  void startUpHandle(Server server);
  class HandlerError : std::exception {};

 private:
  int epoll_fd_;
  struct epoll_event *ev_list_;
  size_t max_event_size_;

  void add(int fd, int event);
  void del(int fd);
  struct epoll_event customEpollEvent(int fd, int event);
  int connectToUpStreamServer(void);
};

#endif
