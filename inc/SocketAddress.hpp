#ifndef WEBSERV_SOCKET_ADDRESS_H
#define WEBSERV_SOCKET_ADDRESS_H

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

 private:
  std::string ip_addr_;
  std::string port_;

  SocketAddress();
};

std::ostream& operator<<(std::ostream& os, const SocketAddress& socket_address);

#endif
