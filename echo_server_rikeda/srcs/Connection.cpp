#include "Connection.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

Connection::Connection(int up_stream_fd, int down_stream_fd,
                       t_connection_type type)
    : up_stream_fd_(up_stream_fd),
      down_stream_fd_(down_stream_fd),
      type_(type) {
  std::cout << "Connect" << std::endl;
}

Connection::~Connection(void) {
  close(up_stream_fd_);
  close(down_stream_fd_);
  std::cout << "Connection Close" << std::endl;
}

void Connection::recvToBuffer(int fd) {
  int bytes = recv(fd, this->buffer, BUFFER_SIZE, 0);
  this->buffer[bytes] = '\0';
}

void Connection::sendBufferContents(int fd) {
  send(fd, this->buffer, std::strlen(this->buffer), 0);
}

int Connection::getUpStreamFd(void) const { return up_stream_fd_; }

int Connection::getDownStreamFd(void) const { return down_stream_fd_; }

Connection::t_connection_type Connection::getConnectionType(void) const {
  return type_;
}
