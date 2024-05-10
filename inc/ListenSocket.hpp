#ifndef WEBSERV_LISTEN_SOCKET_H
#define WEBSERV_LISTEN_SOCKET_H

#include <ostream>

#include "ASocket.hpp"
#include "config/ServerConfig.hpp"

class ListenSocket : public ASocket {
 public:
  ListenSocket(int socket_fd, const SocketAddress& local_address);
  ~ListenSocket();

  int handler(Server* server);

  void setServerConfigs(const std::vector<const ServerConfig*>& server_configs);
  std::vector<const ServerConfig*> getServerConfigs() const;

 private:
  std::vector<const ServerConfig*> server_configs_;

  ListenSocket();
  ListenSocket(const ListenSocket&);
  ListenSocket& operator=(const ListenSocket&);
};

std::ostream& operator<<(std::ostream& os, const ListenSocket& listen_socket);

#endif
