#ifndef WEBSERV_EVENT_MANAGER_H
#define WEBSERV_EVENT_MANAGER_H

#include <deque>
#include <vector>

class ASocket;
class Connection;
class ListenSocket;

class EventManager {
 private:
  int ep_fd_;

  static const int EVLIST_MAX_SIZE = 1024;
  static struct epoll_event ready_list[EVLIST_MAX_SIZE];

 public:
  EventManager(const std::vector<ListenSocket*>& listen_sockets);
  ~EventManager();

  typedef enum RW_FLAG {
    READ,
    WRITE,
  } t_rw_flag;

  int wait(std::deque<ASocket*>& handling_queue);
  int insert(int fd, Connection* connection, t_rw_flag rw_flag);
  int modify(int fd, Connection* connection, t_rw_flag new_rw_flag);
  int erase(int fd);

 private:
  EventManager();
  EventManager(const EventManager&);
  EventManager& operator=(const EventManager&);
};

#endif
