#ifndef WEBSERV_LISTEN_SOCKET_H
#define WEBSERV_LISTEN_SOCKET_H

#include "ASocket.hpp"

class ListenSocket : public ASocket {
 public:
  ListenSocket(int socket_fd);
  ~ListenSocket();

  int handler(Server* server, EventManager* event_manager);
  int errorHandler(Server* server);

 private:
  ListenSocket();
  ListenSocket(const ListenSocket&);
  ListenSocket& operator=(const ListenSocket&);
};

#endif
