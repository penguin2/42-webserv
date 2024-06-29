#include "ASocket.hpp"

int ASocket::getSocketFd() const { return socket_fd_; }

Time ASocket::getTimeout() const { return timeout_; }

void ASocket::setTimeout(const Time& timeout) { timeout_ = timeout; }

bool ASocket::isEventType(int event_type) const {
  return (event_type_flag_ & event_type) != 0;
}

void ASocket::setEventType(int event_type) { event_type_flag_ |= event_type; }

void ASocket::unsetEventType(int event_type) {
  event_type_flag_ &= ~event_type;
}

SocketAddress ASocket::getLocalAddress() const { return local_address_; }

ASocket::ASocket(int socket_fd, const SocketAddress& local_address)
    : socket_fd_(socket_fd),
      local_address_(local_address),
      event_type_flag_(0) {}

ASocket::~ASocket() {}

std::ostream& operator<<(std::ostream& os, const ASocket& socket) {
  return os << "fd: " << socket.getSocketFd() << " (local) "
            << socket.getLocalAddress();
}
