#include "Server.hpp"

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <vector>

#include "Connection.hpp"
#include "Logger.hpp"

Server::Server(const char* config_file) {
  (void)config_file;
  testInitListenSockets(sockets_);
  event_manager_ = new EventManager(sockets_);
  timeout_manager_ = new TimeoutManager();
}

Server::~Server() {
  delete event_manager_;
  delete timeout_manager_;

  for (std::map<int, ASocket*>::iterator it = sockets_.begin();
       it != sockets_.end(); ++it) {
    ASocket* socket = it->second;
    close(socket->getSocketFd());
    delete socket;
  }
}

int Server::acceptListenSocket(int listen_socket_fd) {
  const int connected_socket_fd = accept(listen_socket_fd, NULL, NULL);
  if (connected_socket_fd < 0) return -1;
  if (Server::addNonblockingFlag(connected_socket_fd) < 0) {
    close(connected_socket_fd);
    return -1;
  }
  if (addConnection(connected_socket_fd) < 0) {
    close(connected_socket_fd);
    return -1;
  }
  LOG(INFO, "connected: fd: ", connected_socket_fd);
  return 0;
}

int Server::updateTimeout(ASocket* socket) {
  return timeout_manager_->update(socket);
}

int Server::closeSocket(ASocket* socket) {
  const int closing_socket_fd = socket->getSocketFd();

  event_manager_->erase(closing_socket_fd);
  timeout_manager_->erase(socket);
  sockets_.erase(closing_socket_fd);
  close(closing_socket_fd);
  delete socket;

  LOG(INFO, "closed: fd: ", closing_socket_fd);
  return 0;
}

int Server::start() {
  LOG(INFO, "server: ", "start()");

  if (loop() < 0) return -1;

  return 0;
}

int Server::loop() {
  LOG(INFO, "server: ", "loop()");
  std::vector<ASocket*> timeouts;
  std::deque<ASocket*> events, events_error;
  while (true) {
    timeouts.clear();
    timeout_manager_->addTimeouts(timeouts);
    executeTimeouts(timeouts);

    events.clear();
    events_error.clear();
    const int wait_status = event_manager_->wait(events, events_error);
    if (wait_status < 0) return -1;
    if (wait_status == 0) continue;
    executeEventsErrorQueue(events_error);
    executeEventsQueue(events);
  }
  return 0;
}

int Server::addConnection(int connected_socket_fd) {
  if (sockets_.find(connected_socket_fd) != sockets_.end()) {
    LOG(WARN, "addConnection: ", "already occupied fd!");
    return -1;
  }

  Connection* new_connection = new Connection(connected_socket_fd);
  if (event_manager_->insert(connected_socket_fd, new_connection,
                             EventManager::kRead) < 0 ||
      timeout_manager_->insert(new_connection) < 0) {
    delete new_connection;
    return -1;
  }
  sockets_[connected_socket_fd] = new_connection;
  return 0;
}

int Server::executeEventsErrorQueue(std::deque<ASocket*>& events_error) {
  while (!events_error.empty()) {
    ASocket* socket = events_error.front();
    events_error.pop_front();
    socket->errorHandler(this);
  }
  return 0;
}

int Server::executeEventsQueue(std::deque<ASocket*>& events) {
  while (!events.empty()) {
    ASocket* socket = events.front();
    events.pop_front();
    if (socket->handler(this, event_manager_) == 0) events.push_back(socket);
  }
  return 0;
}

int Server::executeTimeouts(const std::vector<ASocket*>& timeouts) {
  for (std::vector<ASocket*>::const_iterator it = timeouts.begin();
       it != timeouts.end(); ++it) {
    LOG(INFO, "timeout: fd: ", (*it)->getSocketFd());
    closeSocket(*it);
  }
  return 0;
}

int Server::makeListenSocket(const std::string& port) {
  int socket_fd;
  struct addrinfo hints, *listp, *p;

  std::memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  const int status = getaddrinfo(NULL, port.c_str(), &hints, &listp);
  if (status != 0) {
    LOG(WARN, "getaddrinfo: ", gai_strerror(status));
    return -1;
  }

  const int enable = 1;
  for (p = listp; p != NULL; p = p->ai_next) {
    socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socket_fd < 0) continue;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
        0) {
      LOG(WARN, "setsockopt: ", std::strerror(errno));
    }
    if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == 0) break;
    if (close(socket_fd) < 0) {
      LOG(WARN, "close: ", std::strerror(errno));
      freeaddrinfo(listp);
      return -1;
    }
  }
  freeaddrinfo(listp);
  if (p == NULL || addNonblockingFlag(socket_fd) < 0) return -1;

  if (listen(socket_fd, Server::kDefaultListenBacklog) < 0) {
    LOG(WARN, "listen: ", std::strerror(errno));
    close(socket_fd);
    return -1;
  }
  return socket_fd;
}

int Server::addNonblockingFlag(int fd) {
  const int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    LOG(WARN, "fcntl(F_GETFL): ", std::strerror(errno));
    return -1;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    LOG(WARN, "fcntl(F_SETFL): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

// TODO: delete test initializing listen_sockets_
#include "ListenSocket.hpp"
int Server::testInitListenSockets(std::map<int, ASocket*>& sockets) {
  const int listen_socket_fd = Server::makeListenSocket("4242");
  if (listen_socket_fd < 0) return -1;

  ListenSocket* new_listen_socket = new ListenSocket(listen_socket_fd);
  sockets[listen_socket_fd] = new_listen_socket;
  return 0;
}
