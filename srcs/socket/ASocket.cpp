#include "ASocket.hpp"

int ASocket::getSocketFd() const { return socket_fd_; }

ASocket::ASocket(int socket_fd) : socket_fd_(socket_fd) {}

ASocket::~ASocket() {}
