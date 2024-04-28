#ifndef WEBSERV_EVENT_MANAGER_EPOLL_H
#define WEBSERV_EVENT_MANAGER_EPOLL_H

#include <map>
#include <vector>

#include "ASocket.hpp"
#include "EventType.hpp"

class EventManagerEpoll {
 public:
  EventManagerEpoll(const std::map<int, ASocket*>& sockets);
  ~EventManagerEpoll();

  int wait(std::vector<ASocket*>& event_sockets,
           std::vector<ASocket*>& closing_sockets);
  int insert(int fd, ASocket* connection, int event_type);
  int modify(int fd, ASocket* connection, int new_event_type);
  int erase(int fd);

 private:
  int ep_fd_;

  static const int kWaitTimeoutMilli = 500;
  static const int kEvlistMaxSize = 1024;
  static struct epoll_event ready_list_[kEvlistMaxSize];

  static int getEpollEventType(int event_type);
  static int convertEpollEventType(int epoll_event_type);

  EventManagerEpoll();
  EventManagerEpoll(const EventManagerEpoll&);
  EventManagerEpoll& operator=(const EventManagerEpoll&);
};

#endif
