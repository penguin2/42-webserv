#ifndef WEBSERV_SOCKET_ADDRESS_H
#define WEBSERV_SOCKET_ADDRESS_H

#include <sys/socket.h>

#include <ostream>
#include <string>

class SocketAddress {
 public:
  SocketAddress(std::string ip_addr, std::string port);
  SocketAddress(const SocketAddress& other);
  SocketAddress& operator=(const SocketAddress& other);
  ~SocketAddress();

  std::string getIpAddr() const;
  std::string getPort() const;

  bool operator==(const SocketAddress& other) const;
  bool operator<(const SocketAddress& other) const;

  static SocketAddress createFromSockaddrIn(const struct sockaddr_in* addr,
                                            socklen_t addr_len);
  static std::string ipv4ToString(unsigned long ip_long);

 private:
  std::string ip_addr_;
  std::string port_;

  SocketAddress();
};

std::ostream& operator<<(std::ostream& os, const SocketAddress& socket_address);

#endif
