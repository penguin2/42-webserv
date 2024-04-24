#include "ASocket.hpp"

int ASocket::getSocketFd() const { return socket_fd_; }

Time ASocket::getTimeout() const { return timeout_; }

void ASocket::setTimeout(const Time& timeout) { timeout_ = timeout; }

int ASocket::getEventType() const { return event_type_; }

void ASocket::setEventType(int event_type) { event_type_ = event_type; }

ASocket::ASocket(int socket_fd) : socket_fd_(socket_fd) {}

ASocket::~ASocket() {}
