#include "ListenSocket.hpp"

#include "Server.hpp"

ListenSocket::ListenSocket(int socket_fd) : ASocket(socket_fd) {}

ListenSocket::~ListenSocket() {}

int ListenSocket::handler(Server* server, EventManager* event_manager) {
  (void)event_manager;
  return server->acceptListenSocket(socket_fd_);
}

int ListenSocket::errorHandler(Server* server) {
  return server->closeSocket(this);
}
