#include "ASocket.hpp"

int ASocket::getSocketFd() const { return socket_fd_; }

Time ASocket::getTimeout() const { return timeout_; }

void ASocket::setTimeout(const Time& timeout) { timeout_ = timeout; }

int ASocket::getEventType() const { return event_type_; }

void ASocket::setEventType(int event_type) { event_type_ = event_type; }

SocketAddress ASocket::getLocalAddress() const { return local_address_; }

ASocket::ASocket(int socket_fd, const SocketAddress& local_address)
    : socket_fd_(socket_fd), local_address_(local_address) {}

ASocket::~ASocket() {}

std::ostream& operator<<(std::ostream& os, const ASocket& socket) {
  return os << "fd: " << socket.getSocketFd() << " (local) "
            << socket.getLocalAddress();
}
