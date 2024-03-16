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

#include "AHandler.hpp"
#include "Connection.hpp"
#include "ConnectionManager.hpp"

ProxyHandler::ProxyHandler(size_t max_event_size) : AHandler(max_event_size) {}

ProxyHandler::~ProxyHandler(void) {}

int ProxyHandler::connectToUpStreamServer() {
  int up_stream_server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (up_stream_server_fd < 0) throw HandlerError();
  int flags = fcntl(up_stream_server_fd, F_GETFL, 0);
  fcntl(up_stream_server_fd, F_SETFL, flags | O_NONBLOCK);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(SERVER_PORT);
  addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
  if (connect(up_stream_server_fd, (struct sockaddr*)&addr, sizeof(addr)) ==
          -1 &&
      errno != EINPROGRESS)
    throw HandlerError();
  return up_stream_server_fd;
}

void ProxyHandler::startUpHandle(Server server) {
  ConnectionManager manager;

  add(server.getListenFd(), EPOLLIN);
  while (true) {
    int nfds = epoll_wait(epoll_fd_, ev_list_, max_event_size_, -1);
    if (nfds < 0) throw HandlerError();
    for (int i = 0; i < nfds; i++) {
      if (ev_list_[i].data.fd == server.getListenFd()) {
        int fd = accept(server.getListenFd(), NULL, NULL);
        if (fd < 0) throw HandlerError();
        add(fd, EPOLLIN);
        manager.add(-1, fd);
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
