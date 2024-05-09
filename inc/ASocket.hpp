#ifndef WEBSERV_ASOCKET_H
#define WEBSERV_ASOCKET_H

#include <ostream>

#include "SocketAddress.hpp"
#include "Time.hpp"

class Server;

class ASocket {
 public:
  int getSocketFd() const;

  Time getTimeout() const;
  void setTimeout(const Time& timeout);

  int getEventType() const;
  void setEventType(int event_type);

  SocketAddress getLocalAddress() const;

  virtual int handler(Server* server) = 0;
  virtual ~ASocket();

 protected:
  int socket_fd_;

  ASocket(int socket_fd, const SocketAddress& local_address);

 private:
  SocketAddress local_address_;
  Time timeout_;
  int event_type_;

  ASocket();
  ASocket(const ASocket&);
  ASocket& operator=(const ASocket&);
};

std::ostream& operator<<(std::ostream& os, const ASocket& socket);

#endif
