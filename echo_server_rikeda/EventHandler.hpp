#ifndef Wevserv_EventHandler_H_
#define Wevserv_EventHandler_H_
#include <exception>
#include <string>

#include "EchoServer.hpp"

class EventHandler {
 public:
  EventHandler(size_t max_event_size);
  ~EventHandler(void);

  void startUpHandleServer(EchoServer server);
  class EventHandlerError : std::exception {};

 private:
  const static size_t BUFFER_SIZE = 10000;
  char buffer[BUFFER_SIZE];
  int epoll_fd_;
  struct epoll_event *ev_list_;
  size_t max_event_size_;

  void add(int fd, int event);
  void del(int fd);
  struct epoll_event customEpollEvent(int fd, int event);
};

#endif
