#ifndef WEBSERV_LISTEN_SOCKET_H
#define WEBSERV_LISTEN_SOCKET_H

#include <ostream>

#include "ASocket.hpp"

class ListenSocket : public ASocket {
 public:
  ListenSocket(int socket_fd, const SocketAddress& local_address);
  ~ListenSocket();

  int handler(Server* server);

 private:
  ListenSocket();
  ListenSocket(const ListenSocket&);
  ListenSocket& operator=(const ListenSocket&);
};

std::ostream& operator<<(std::ostream& os, const ListenSocket& listen_socket);

#endif
