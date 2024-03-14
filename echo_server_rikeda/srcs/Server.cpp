#include "Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "EventHandler.hpp"

Server::Server(const char *host, unsigned short port) {
  listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd_ < 0) throw ServerInternalError();
  struct sockaddr_in addr = customSockaddr(host, port);
  if (bind(listen_fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    throw ServerInternalError();
  if (listen(listen_fd_, 3) < 0) throw ServerInternalError();
  std::cout << host << ":" << port << std::endl;
}

Server::~Server(void) { close(listen_fd_); }

struct sockaddr_in Server::customSockaddr(const char *host,
                                          unsigned short port) {
  struct sockaddr_in a_addr;
  std::memset(&a_addr, 0, sizeof(a_addr));
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons(port);
  a_addr.sin_addr.s_addr = inet_addr(host);
  return a_addr;
}

int Server::getListenFd(void) const { return listen_fd_; }
