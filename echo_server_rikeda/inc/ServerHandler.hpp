#ifndef Wevserv_ServerHandler_H_
#define Wevserv_ServerHandler_H_
#include <exception>
#include <string>

#include "Server.hpp"

class ServerHandler {
 public:
  ServerHandler(size_t max_event_size);
  ~ServerHandler(void);

  void startUpHandle(Server server);
  class HandlerError : std::exception {};

 private:
  int epoll_fd_;
  struct epoll_event *ev_list_;
  size_t max_event_size_;

  void add(int fd, int event);
  void del(int fd);
  struct epoll_event customEpollEvent(int fd, int event);
};

#endif
