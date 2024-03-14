#include "ProxyHandler.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "Connection.hpp"
#include "ConnectionManager.hpp"

ProxyHandler::ProxyHandler(size_t max_event_size)
    : max_event_size_(max_event_size) {
  ev_list_ = new struct epoll_event[max_event_size];
  epoll_fd_ = epoll_create(max_event_size);
  if (epoll_fd_ < 0) throw ProxyHandlerError();
}

ProxyHandler::~ProxyHandler(void) { delete[] ev_list_; }

void ProxyHandler::add(int fd, int event) {
  struct epoll_event ev = customEpollEvent(fd, event);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0)
    throw ProxyHandlerError();
}

void ProxyHandler::del(int fd) {
  struct epoll_event ev = customEpollEvent(fd, EPOLLIN);
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev) < 0)
    throw ProxyHandlerError();
}

int ProxyHandler::connectToUpStreamServer() {
  int up_stream_server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (up_stream_server_fd < 0) throw ProxyHandler::ProxyHandlerError();
  int flags = fcntl(up_stream_server_fd, F_GETFL, 0);
  fcntl(up_stream_server_fd, F_SETFL, flags | O_NONBLOCK);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4243);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(up_stream_server_fd, (struct sockaddr*)&addr, sizeof(addr)) ==
          -1 &&
      errno != EINPROGRESS)
    throw ProxyHandler::ProxyHandlerError();
  return up_stream_server_fd;
}

void ProxyHandler::startUpHandleProxy(Server server) {
  ConnectionManager manager;

  add(server.getListenFd(), EPOLLIN);
  while (true) {
    int nfds = epoll_wait(epoll_fd_, ev_list_, max_event_size_, -1);
    if (nfds < 0) throw ProxyHandlerError();
    for (int i = 0; i < nfds; i++) {
      if (ev_list_[i].data.fd == server.getListenFd()) {
        int fd = accept(server.getListenFd(), NULL, NULL);
        if (fd < 0) throw ProxyHandlerError();
        add(fd, EPOLLIN);
        manager.add(-1, fd, Connection::CLIENT_SERVER);
      } else if (ev_list_[i].events & EPOLLIN) {
        Connection* conn = manager.searchFromDownStreamFds(ev_list_[i].data.fd);
        if (conn != NULL) {
          conn->recvToBuffer(conn->getDownStreamFd());
          del(conn->getDownStreamFd());
          int fd = connectToUpStreamServer();
          add(fd, EPOLLOUT);
          conn->setUpStreamFd(fd);
        } else {
          conn = manager.searchFromUpStreamFds(ev_list_[i].data.fd);
          conn->recvToBuffer(conn->getUpStreamFd());
          del(conn->getUpStreamFd());
          add(conn->getDownStreamFd(), EPOLLOUT);
        }
      } else {
        Connection* conn = manager.searchFromDownStreamFds(ev_list_[i].data.fd);
        if (conn != NULL) {
          conn->sendBufferContents(conn->getDownStreamFd());
          // conn->sendResponse(conn->getDownStreamFd());
          del(conn->getDownStreamFd());
          manager.del(conn->getDownStreamFd());
        } else {
          conn = manager.searchFromUpStreamFds(ev_list_[i].data.fd);
          conn->sendBufferContents(conn->getUpStreamFd());
          del(conn->getUpStreamFd());
          add(conn->getUpStreamFd(), EPOLLIN);
        }
      }
    }
  }
}

struct epoll_event ProxyHandler::customEpollEvent(int fd, int event) {
  struct epoll_event ev;
  std::memset(&ev, 0, sizeof(ev));
  ev.events = event;
  ev.data.fd = fd;
  return ev;
}
