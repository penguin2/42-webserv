#ifndef WEBSERV_LISTEN_SOCKET_H
#define WEBSERV_LISTEN_SOCKET_H

#include <string>

#include "ASocket.hpp"

class Server;

class ListenSocket : ASocket {
 private:
  Server* server_;

 public:
  ListenSocket(int socket_fd, Server* server);
  ~ListenSocket();

  int handler();

  static int openListenFd(const std::string& port);

 private:
  ListenSocket();
  ListenSocket(const ListenSocket&);
  ListenSocket& operator=(const ListenSocket&);
};

#endif
