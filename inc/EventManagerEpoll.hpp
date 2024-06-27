#ifndef WEBSERV_EVENT_MANAGER_EPOLL_H
#define WEBSERV_EVENT_MANAGER_EPOLL_H
#if defined(__linux__)

#include <map>
#include <set>

#include "ASocket.hpp"
#include "EventType.hpp"

class EventManagerEpoll {
 public:
  EventManagerEpoll(const std::map<int, ASocket*>& sockets);
  ~EventManagerEpoll();

  int wait(std::set<ASocket*>& event_sockets,
           std::set<ASocket*>& closing_sockets);
  int insert(int fd, ASocket* socket, int event_type);
  int modify(int fd, ASocket* socket, int new_event_type);
  int erase(int fd, ASocket* socket, int event_type);

 private:
  int ep_fd_;

  static const int kWaitTimeoutMilli = 500;
  static const int kEvlistMaxSize = 1024;
  static struct epoll_event ready_list_[kEvlistMaxSize];

  static int makeEpollFlags(int event_type);
  static int makeEventType(int epoll_flags);

  EventManagerEpoll();
  EventManagerEpoll(const EventManagerEpoll&);
  EventManagerEpoll& operator=(const EventManagerEpoll&);
};

#endif
#endif
