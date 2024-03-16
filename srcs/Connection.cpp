#include "Connection.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <sstream>

Connection::Connection(int up_stream_fd, int down_stream_fd)
    : up_stream_fd_(up_stream_fd), down_stream_fd_(down_stream_fd) {
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

void Connection::sendResponse(int fd) {
  std::string buffer_string(buffer);
  std::ostringstream oss;
  oss << buffer_string.size();
  std::string response = std::string("HTTP/1.1 OK\n") +
                         std::string("Content-Type: text/pre") +
                         std::string("Content-Length: ") + oss.str() +
                         std::string("\n\n") + buffer_string;
  send(fd, response.c_str(), response.size(), 0);
}

int Connection::getUpStreamFd(void) const { return up_stream_fd_; }

int Connection::getDownStreamFd(void) const { return down_stream_fd_; }

void Connection::setUpStreamFd(int fd) { this->up_stream_fd_ = fd; }

void Connection::setDownStreamFd(int fd) { this->down_stream_fd_ = fd; }
