#include "ListenSocket.hpp"

#include "Logger.hpp"
#include "Server.hpp"

ListenSocket::ListenSocket(int socket_fd) : ASocket(socket_fd) {}

ListenSocket::~ListenSocket() {}

int ListenSocket::handler(Server* server, EventManager* event_manager) {
  (void)event_manager;
  if (server->acceptListenSocket(socket_fd_) < 0)
    LOG(WARN, "accept() failed on  ListenSocket handler: fd: ", socket_fd_);
  return 0;
}
