#include "ListenSocket.hpp"

#include "Logger.hpp"
#include "Server.hpp"

ListenSocket::ListenSocket(int socket_fd, const SocketAddress& local_address)
    : ASocket(socket_fd, local_address) {}

ListenSocket::~ListenSocket() {}

int ListenSocket::handler(Server* server) {
  if (server->acceptListenSocket(socket_fd_) < 0)
    LOG(WARN, "accepting connection failed on ListenSocket: ", *this);
  return 0;
}

std::ostream& operator<<(std::ostream& os, const ListenSocket& listen_socket) {
  return os << "fd: " << listen_socket.getSocketFd() << " (listening) "
            << listen_socket.getLocalAddress();
}
