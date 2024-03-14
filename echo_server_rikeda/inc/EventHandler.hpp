#ifndef Wevserv_EventHandler_H_
#define Wevserv_EventHandler_H_
#include <exception>
#include <string>

#include "Server.hpp"

class EventHandler {
 public:
  EventHandler(size_t max_event_size);
  ~EventHandler(void);

  void startUpHandleServer(Server server);
  class EventHandlerError : std::exception {};

 private:
  int epoll_fd_;
  struct epoll_event *ev_list_;
  size_t max_event_size_;

  void add(int fd, int event);
  void del(int fd);
  struct epoll_event customEpollEvent(int fd, int event);
};

#endif
