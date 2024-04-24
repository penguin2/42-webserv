#ifndef WEBSERV_EVENT_MANAGER_H
#define WEBSERV_EVENT_MANAGER_H

#include <map>
#include <vector>

#include "ASocket.hpp"

class EventManager {
 public:
  EventManager(const std::map<int, ASocket*>& sockets);
  ~EventManager();

  int wait(std::vector<ASocket*>& event_sockets,
           std::vector<ASocket*>& closing_sockets);
  int insert(int fd, ASocket* connection, int event_type);
  int modify(int fd, ASocket* connection, int new_event_type);
  int erase(int fd);

  // bits for event type
  static const int kEventTypeRead = 1;
  static const int kEventTypeWrite = 2;

  static bool isReadEvent(int event_type);
  static bool isWriteEvent(int event_type);

 private:
  int ep_fd_;

  static const int kWaitTimeoutMilli = 500;
  static const int kEvlistMaxSize = 1024;
  static struct epoll_event ready_list_[kEvlistMaxSize];

  static int getEpollEventType(int event_type);
  static int convertEpollEventType(int epoll_event_type);

  EventManager();
  EventManager(const EventManager&);
  EventManager& operator=(const EventManager&);
};

#endif
