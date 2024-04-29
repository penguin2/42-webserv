#ifndef WEBSERV_EVENT_MANAGER_KQUEUE_H
#define WEBSERV_EVENT_MANAGER_KQUEUE_H

#include <map>
#include <vector>

#include "ASocket.hpp"
#include "EventType.hpp"

class EventManagerKqueue {
 public:
  EventManagerKqueue(const std::map<int, ASocket*>& sockets);
  ~EventManagerKqueue();

  int wait(std::vector<ASocket*>& event_sockets,
           std::vector<ASocket*>& closing_sockets);
  int insert(int fd, ASocket* socket, int event_type);
  int modify(int fd, ASocket* socket, int new_event_type);
  int erase(int fd, ASocket* socket, int event_type);

 private:
  int kq_fd_;

  static const int kWaitTimeoutSec = 0;
  static const int kWaitTimeoutNanoSec = 500000000;
  static const struct timespec kWaitTimeout;
  static const int kEvlistMaxSize = 1024;
  static struct kevent ready_list_[kEvlistMaxSize];

  static int makeKqueueFilter(int event_type);
  static int makeEventType(int kqueue_filter);

  EventManagerKqueue();
  EventManagerKqueue(const EventManagerKqueue&);
  EventManagerKqueue& operator=(const EventManagerKqueue&);
};

#endif
