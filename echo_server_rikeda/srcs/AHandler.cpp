#include "AHandler.hpp"

#include <sys/epoll.h>

#include <cstring>

AHandler::AHandler(size_t max_event_size) : max_event_size_(max_event_size) {
  ev_list_ = new struct epoll_event[max_event_size];
  epoll_fd_ = epoll_create(max_event_size);
  if (epoll_fd_ < 0) throw HandlerError();
}

AHandler::~AHandler(void) { delete[] ev_list_; }

void AHandler::add(int fd, int event) {
  struct epoll_event ev = customEpollEvent(fd, event);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) throw HandlerError();
}

void AHandler::del(int fd) {
  struct epoll_event ev = customEpollEvent(fd, EPOLLIN);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev) < 0) throw HandlerError();
}
struct epoll_event AHandler::customEpollEvent(int fd, int event) {
  struct epoll_event ev;
  std::memset(&ev, 0, sizeof(ev));
  ev.events = event;
  ev.data.fd = fd;
  return ev;
}
