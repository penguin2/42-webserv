#include "Server.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <vector>

#include "Connection.hpp"
#include "Logger.hpp"
#include "SocketAddress.hpp"
#include "SysUtils.hpp"
#include "config/ConfigAdapter.hpp"

Server::Server(const char* config_file) {
  (void)config_file;
  sockets_ = ConfigAdapter::makeInitialListenSockets();
  event_manager_ = new EventManager(sockets_);
  timeout_manager_ = new TimeoutManager();
}

Server::~Server() {
  for (std::map<int, ASocket*>::iterator it = sockets_.begin();
       it != sockets_.end(); ++it) {
    ASocket* socket = it->second;
    close(socket->getSocketFd());
    delete socket;
  }
  delete event_manager_;
  delete timeout_manager_;
}

int Server::acceptListenSocket(int listen_socket_fd) {
  struct sockaddr peer_sockaddr;
  socklen_t peer_sockaddr_len = sizeof(struct sockaddr);
  struct sockaddr local_sockaddr;
  socklen_t local_sockaddr_len = sizeof(struct sockaddr);

  const int connected_socket_fd =
      accept(listen_socket_fd, &peer_sockaddr, &peer_sockaddr_len);
  if (connected_socket_fd < 0) return -1;
  if (getsockname(connected_socket_fd, &local_sockaddr, &local_sockaddr_len) <
      0) {
    close(connected_socket_fd);
    return -1;
  }

  const SocketAddress local_address = SocketAddress::createFromSockaddrIn(
      reinterpret_cast<struct sockaddr_in*>(&local_sockaddr),
      local_sockaddr_len);
  const SocketAddress peer_address = SocketAddress::createFromSockaddrIn(
      reinterpret_cast<struct sockaddr_in*>(&peer_sockaddr), peer_sockaddr_len);

  if (SysUtils::addNonblockingFlag(connected_socket_fd) < 0 ||
      SysUtils::addCloseOnExecFlag(connected_socket_fd) < 0 ||
      addConnection(connected_socket_fd, local_address, peer_address) < 0) {
    close(connected_socket_fd);
    return -1;
  }
  return 0;
}

int Server::updateTimeout(ASocket* socket) {
  return timeout_manager_->update(socket);
}

int Server::start() {
  LOG(INFO, "server: ", "start()");

  Connection::initTransitHandlers();

  if (loop() < 0) return -1;

  return 0;
}

int Server::loop() {
  LOG(INFO, "server: ", "loop()");
  std::vector<ASocket*> event_sockets, closing_sockets;
  while (true) {
    closeSockets(timeout_manager_->findTimeouts());

    closing_sockets.clear();
    event_sockets.clear();
    const int wait_status =
        event_manager_->wait(event_sockets, closing_sockets);
    if (wait_status < 0) return -1;
    if (wait_status == 0) continue;
    executeEventSockets(event_sockets, closing_sockets);
    closeSockets(closing_sockets);
  }
  return 0;
}

int Server::addConnection(int connected_socket_fd,
                          const SocketAddress& local_address,
                          const SocketAddress& peer_address) {
  if (sockets_.find(connected_socket_fd) != sockets_.end()) {
    LOG(WARN, "addConnection: duplicated fd: ", connected_socket_fd);
    return -1;
  }

  Connection* new_connection = new Connection(
      connected_socket_fd, local_address, peer_address, event_manager_);

  if (event_manager_->insert(connected_socket_fd, new_connection,
                             EventType::READ) < 0) {
    delete new_connection;
    return -1;
  }

  if (timeout_manager_->insert(new_connection) < 0) {
    event_manager_->erase(connected_socket_fd, new_connection, EventType::READ);
    delete new_connection;
    return -1;
  }

  sockets_[connected_socket_fd] = new_connection;

  LOG(INFO, "connection created: ", *new_connection);
  return 0;
}

int Server::executeEventSockets(const std::vector<ASocket*>& event_sockets,
                                std::vector<ASocket*>& closing_sockets) {
  for (std::vector<ASocket*>::const_iterator it = event_sockets.begin();
       it != event_sockets.end(); ++it) {
    ASocket* event_socket = *it;
    if (event_socket->handler(this) < 0)
      closing_sockets.push_back(event_socket);
  }
  return 0;
}

int Server::closeSocket(ASocket* socket) {
  const int closing_socket_fd = socket->getSocketFd();

  event_manager_->erase(closing_socket_fd, socket, EventType::READ);
  event_manager_->erase(closing_socket_fd, socket, EventType::WRITE);
  timeout_manager_->erase(socket);
  sockets_.erase(closing_socket_fd);
  close(closing_socket_fd);

  LOG(INFO, "closed: ", *socket);
  delete socket;

  return 0;
}

int Server::closeSockets(const std::vector<ASocket*>& closing_sockets) {
  for (std::vector<ASocket*>::const_iterator it = closing_sockets.begin();
       it != closing_sockets.end(); ++it) {
    closeSocket(*it);
  }
  return 0;
}
