#include "EventManagerEpoll.hpp"

#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "ASocket.hpp"
#include "Logger.hpp"
#include "SysUtils.hpp"

struct epoll_event
    EventManagerEpoll::ready_list_[EventManagerEpoll::kEvlistMaxSize];

EventManagerEpoll::EventManagerEpoll(
    const std::map<int, ASocket*>& listen_sockets) {
  ep_fd_ = epoll_create(2);
  if (ep_fd_ < 0 || SysUtils::addCloseOnExecFlag(ep_fd_) < 0) {
    LOG(ERROR, "epoll_create: ", std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }
  for (std::map<int, ASocket*>::const_iterator it = listen_sockets.begin();
       it != listen_sockets.end(); ++it) {
    int fd = it->first;
    ASocket* socket = it->second;
    if (insert(fd, socket, EventType::READ) < 0) {
      LOG(ERROR,
          "EventManagerEpoll(constructor): ", "failed on initializing sockets");
      std::exit(EXIT_FAILURE);
    }
  }
}

EventManagerEpoll::~EventManagerEpoll() { close(ep_fd_); }

int EventManagerEpoll::wait(std::vector<ASocket*>& event_sockets,
                            std::vector<ASocket*>& closing_sockets) {
  const int ready_list_size = epoll_wait(ep_fd_, EventManagerEpoll::ready_list_,
                                         EventManagerEpoll::kEvlistMaxSize,
                                         EventManagerEpoll::kWaitTimeoutMilli);
  if (ready_list_size < 0) {
    LOG(ERROR, "epoll_wait: ", std::strerror(errno));
    return -1;
  }
  if (ready_list_size == 0) return 0;

  for (int i = 0; i < ready_list_size; ++i) {
    ASocket* current_socket =
        reinterpret_cast<ASocket*>(ready_list_[i].data.ptr);

    if (ready_list_[i].events & (EPOLLHUP | EPOLLRDHUP))
      ready_list_[i].events = EPOLLIN;

    current_socket->setEventType(
        EventManagerEpoll::makeEventType(ready_list_[i].events));

    if (ready_list_[i].events & (EPOLLIN | EPOLLOUT))
      event_sockets.push_back(current_socket);
    else if (ready_list_[i].events & EPOLLERR) {
      closing_sockets.push_back(current_socket);
    }
  }
  return ready_list_size;
}

int EventManagerEpoll::insert(int fd, ASocket* socket, int event_type) {
  struct epoll_event ev;
  ev.events = EventManagerEpoll::makeEpollFlags(event_type);
  ev.data.ptr = reinterpret_cast<void*>(socket);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
    LOG(WARN, "epoll_ctl(insert): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManagerEpoll::modify(int fd, ASocket* socket, int new_event_type) {
  struct epoll_event ev;
  ev.events = EventManagerEpoll::makeEpollFlags(new_event_type);
  ev.data.ptr = reinterpret_cast<void*>(socket);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
    LOG(WARN, "epoll_ctl(modify): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManagerEpoll::erase(int fd) {
  if (epoll_ctl(ep_fd_, EPOLL_CTL_DEL, fd, NULL) < 0) {
    LOG(DEBUG, "epoll_ctl(erase): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManagerEpoll::makeEventType(int epoll_flags) {
  int event_type = 0;
  if (epoll_flags & EPOLLIN) event_type |= EventType::READ;
  if (epoll_flags & EPOLLOUT) event_type |= EventType::WRITE;
  return event_type;
}

int EventManagerEpoll::makeEpollFlags(int event_type) {
  int epoll_flags = 0;
  if (event_type & EventType::READ) epoll_flags |= EPOLLIN;
  if (event_type & EventType::WRITE) epoll_flags |= EPOLLOUT;
  return epoll_flags;
}
