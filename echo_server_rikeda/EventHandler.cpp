#include "EventHandler.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define RESPONSE                                                               \
  "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 20\n\n<p>Hello, " \
  "World!</p>"

EventHandler::EventHandler(size_t max_event_size)
    : max_event_size_(max_event_size) {
  ev_list_ = new struct epoll_event[max_event_size];
  epoll_fd_ = epoll_create(max_event_size);
  if (epoll_fd_ < 0) throw EventHandlerError();
}

EventHandler::~EventHandler(void) { delete[] ev_list_; }

void EventHandler::add(int fd, int event) {
  struct epoll_event ev = customEpollEvent(fd, event);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0)
    throw EventHandlerError();
}

void EventHandler::del(int fd) {
  struct epoll_event ev = customEpollEvent(fd, EPOLLIN);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev) < 0)
    throw EventHandlerError();
}

void EventHandler::startUpHandleServer(EchoServer server) {
  add(server.getListenFd(), EPOLLIN);
  while (true) {
    int nfds = epoll_wait(epoll_fd_, ev_list_, max_event_size_, -1);
    if (nfds < 0) throw EventHandlerError();
    for (int i = 0; i < nfds; i++) {
      if (ev_list_[i].data.fd == server.getListenFd()) {
        int fd = accept(server.getListenFd(), NULL, NULL);
        if (fd < 0) throw EventHandlerError();
        add(fd, EPOLLIN);
      } else if (ev_list_[i].events & EPOLLIN) {
        int fd = ev_list_[i].data.fd;
        int bytes = recv(fd, buffer, BUFFER_SIZE, 0);
        buffer[bytes] = '\0';
        std::cout << buffer << std::endl;
        del(fd);
        add(fd, EPOLLOUT);
      } else {
        int fd = ev_list_[i].data.fd;
        send(fd, RESPONSE, strlen(RESPONSE), 0);
        del(fd);
        close(fd);
      }
    }
  }
}

struct epoll_event EventHandler::customEpollEvent(int fd, int event) {
  struct epoll_event ev;
  std::memset(&ev, 0, sizeof(ev));
  ev.events = event;
  ev.data.fd = fd;
  return ev;
}
