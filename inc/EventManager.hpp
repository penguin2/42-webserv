#ifndef WEBSERV_EVENT_MANAGER_H
#define WEBSERV_EVENT_MANAGER_H

#include <map>
#include <vector>

class ASocket;

class EventManager {
 public:
  EventManager(const std::map<int, ASocket*>& sockets);
  ~EventManager();

  typedef enum eEventFlag { kRead, kWrite } EventFlag;

  int wait(std::vector<ASocket*>& event_sockets,
           std::vector<ASocket*>& closing_sockets);
  int insert(int fd, ASocket* connection, EventFlag event_flag);
  int modify(int fd, ASocket* connection, EventFlag new_event_flag);
  int erase(int fd);

 private:
  int ep_fd_;

  static const int kWaitTimeoutMilli = 500;
  static const int kEvlistMaxSize = 1024;
  static struct epoll_event ready_list_[kEvlistMaxSize];

  EventManager();
  EventManager(const EventManager&);
  EventManager& operator=(const EventManager&);
};

#endif
