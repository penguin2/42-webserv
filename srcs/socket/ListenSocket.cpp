#include "ListenSocket.hpp"

#include "Logger.hpp"
#include "Server.hpp"

ListenSocket::ListenSocket(int socket_fd, const SocketAddress& local_address)
    : ASocket(socket_fd, local_address) {}

ListenSocket::~ListenSocket() {}

int ListenSocket::handler(Server* server) {
  if (server->acceptListenSocket(*this) < 0)
    LOG(WARN, "accepting connection failed on ListenSocket: ", *this);
  return 0;
}

void ListenSocket::setServerConfigs(
    const std::vector<const ServerConfig*>& server_configs) {
  server_configs_ = server_configs;
}

std::vector<const ServerConfig*> ListenSocket::getServerConfigs() const {
  return server_configs_;
}
std::ostream& operator<<(std::ostream& os, const ListenSocket& listen_socket) {
  return os << "fd: " << listen_socket.getSocketFd() << " (listening) "
            << listen_socket.getLocalAddress();
}
