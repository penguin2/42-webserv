#include "SocketAddress.hpp"

#include <netdb.h>

#include <ostream>
#include <vector>

#include "Utils.hpp"

SocketAddress::SocketAddress(std::string ip_addr, std::string port)
    : ip_addr_(ip_addr), port_(port) {}

SocketAddress::SocketAddress(const SocketAddress& other)
    : ip_addr_(other.ip_addr_), port_(other.port_) {}

SocketAddress& SocketAddress::operator=(const SocketAddress& other) {
  if (this != &other) {
    this->ip_addr_ = other.ip_addr_;
    this->port_ = other.port_;
  }
  return *this;
}

SocketAddress::~SocketAddress() {}

std::string SocketAddress::getIpAddr() const { return this->ip_addr_; }

std::string SocketAddress::getPort() const { return this->port_; }

bool SocketAddress::operator==(const SocketAddress& other) const {
  return this->ip_addr_ == other.ip_addr_ && this->port_ == other.port_;
}

bool SocketAddress::operator<(const SocketAddress& other) const {
  const int ip_addr_compared = this->ip_addr_.compare(other.ip_addr_);

  if (ip_addr_compared != 0) return ip_addr_compared < 0;

  return this->port_.compare(other.port_) < 0;
}

SocketAddress SocketAddress::createFromSockaddrIn(
    const struct sockaddr_in* addr, socklen_t addr_len) {
  const std::string ip_addr =
      SocketAddress::ipv4ToString(ntohl(addr->sin_addr.s_addr));
  const std::string port = Utils::uintToString(ntohs(addr->sin_port));

  return SocketAddress(ip_addr, port);
  (void)addr_len;
}

std::string SocketAddress::ipv4ToString(unsigned long ip_long) {
  std::vector<std::string> ip_segments;
  ip_segments.push_back(Utils::uintToString((ip_long >> 24) & 0xffU));
  ip_segments.push_back(Utils::uintToString((ip_long >> 16) & 0xffU));
  ip_segments.push_back(Utils::uintToString((ip_long >> 8) & 0xffU));
  ip_segments.push_back(Utils::uintToString((ip_long >> 0) & 0xffU));
  return Utils::joinStrings(ip_segments, ".");
}

std::ostream& operator<<(std::ostream& os,
                         const SocketAddress& socket_address) {
  return os << socket_address.getIpAddr() << ":" << socket_address.getPort();
}
