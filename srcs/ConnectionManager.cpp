#include "ConnectionManager.hpp"

#include <iostream>

#include "Connection.hpp"

ConnectionManager::ConnectionManager(void) {}

ConnectionManager::~ConnectionManager(void) {}

void ConnectionManager::add(int up_stream_fd, int down_stream_fd) {
  connections.push_back(new Connection(up_stream_fd, down_stream_fd));
}

int ConnectionManager::del(int fd) {
  for (size_t i = 0; i < connections.size(); ++i) {
    if (connections[i]->getDownStreamFd() == fd) {
      delete connections[i];
      connections.erase(connections.begin() + i);
      return 0;
    }
  }
  return -1;
}

Connection* ConnectionManager::searchFromUpStreamFds(int fd) const {
  for (size_t i = 0; i < connections.size(); ++i) {
    if (connections[i]->getUpStreamFd() == fd) return connections[i];
  }
  return NULL;
}

Connection* ConnectionManager::searchFromDownStreamFds(int fd) const {
  for (size_t i = 0; i < connections.size(); ++i) {
    if (connections[i]->getDownStreamFd() == fd) return connections[i];
  }
  return NULL;
}
