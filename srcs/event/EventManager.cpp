#include "EventManager.hpp"

#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "ASocket.hpp"
#include "Logger.hpp"

struct epoll_event EventManager::ready_list_[EventManager::kEvlistMaxSize];

EventManager::EventManager(const std::map<int, ASocket*>& listen_sockets) {
  ep_fd_ = epoll_create(2);
  if (ep_fd_ < 0) {
    LOG(ERROR, "epoll_create: ", std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }
  for (std::map<int, ASocket*>::const_iterator it = listen_sockets.begin();
       it != listen_sockets.end(); ++it) {
    ASocket* current_socket = it->second;
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = current_socket->getSocketFd();
    ev.data.ptr = reinterpret_cast<void*>(current_socket);
    if (epoll_ctl(ep_fd_, EPOLL_CTL_ADD, current_socket->getSocketFd(), &ev) <
        0) {
      LOG(ERROR, "epoll_ctl(listen socket): ", std::strerror(errno));
      std::exit(EXIT_FAILURE);
    }
  }
}

EventManager::~EventManager() { close(ep_fd_); }

int EventManager::wait(std::deque<ASocket*>& events,
                       std::deque<ASocket*>& events_error) {
  const int ready_list_size =
      epoll_wait(ep_fd_, EventManager::ready_list_,
                 EventManager::kEvlistMaxSize, EventManager::kWaitTimeoutMilli);
  if (ready_list_size < 0) {
    LOG(ERROR, "epoll_wait: ", std::strerror(errno));
    return -1;
  }
  if (ready_list_size == 0) return 0;
  for (int i = 0; i < ready_list_size; ++i) {
    ASocket* current_socket =
        reinterpret_cast<ASocket*>(ready_list_[i].data.ptr);
    if (ready_list_[i].events & (EPOLLHUP | EPOLLERR))
      events_error.push_back(current_socket);
    else
      events.push_back(current_socket);
  }
  return ready_list_size;
}

int EventManager::insert(int fd, ASocket* socket, EventFlag event_flag) {
  struct epoll_event ev;
  ev.events = event_flag == EventManager::kRead ? EPOLLIN : EPOLLOUT;
  ev.data.fd = fd;
  ev.data.ptr = reinterpret_cast<void*>(socket);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
    LOG(WARN, "epoll_ctl(insert): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManager::modify(int fd, ASocket* socket, EventFlag new_event_flag) {
  struct epoll_event ev;
  ev.events = new_event_flag == EventManager::kRead ? EPOLLIN : EPOLLOUT;
  ev.data.fd = fd;
  ev.data.ptr = reinterpret_cast<void*>(socket);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
    LOG(WARN, "epoll_ctl(modify): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManager::erase(int fd) {
  if (epoll_ctl(ep_fd_, EPOLL_CTL_DEL, fd, NULL) < 0) {
    LOG(WARN, "epoll_ctl(erase): ", std::strerror(errno));
    return -1;
  }
  return 0;
}
