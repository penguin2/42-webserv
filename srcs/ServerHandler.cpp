#include "ServerHandler.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "AHandler.hpp"
#include "Connection.hpp"
#include "ConnectionManager.hpp"

ServerHandler::ServerHandler(size_t max_event_size)
    : AHandler(max_event_size) {}

ServerHandler::~ServerHandler(void) { delete[] ev_list_; }

void ServerHandler::startUpHandle(Server server) {
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
        conn->recvToBuffer(conn->getDownStreamFd());
        del(conn->getDownStreamFd());
        add(conn->getDownStreamFd(), EPOLLOUT);
      } else {
        Connection* conn = manager.searchFromDownStreamFds(ev_list_[i].data.fd);
        conn->sendResponse(conn->getDownStreamFd());
        // conn->sendBufferContents(conn->getDownStreamFd());
        del(conn->getDownStreamFd());
        manager.del(conn->getDownStreamFd());
      }
    }
  }
}
