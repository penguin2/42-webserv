#ifndef WEBSERV_ASOCKET_H
#define WEBSERV_ASOCKET_H

#include "Time.hpp"
class Server;
class EventManager;

class ASocket {
 public:
  int getSocketFd() const;
  Time getTimeout() const;
  void setTimeout(const Time& timeout);
  virtual int handler(Server* server, EventManager* event_manager) = 0;
  virtual int errorHandler(Server* server) = 0;
  virtual ~ASocket();

 protected:
  int socket_fd_;

  ASocket(int socket_fd);

 private:
  Time timeout_;

  ASocket();
  ASocket(const ASocket&);
  ASocket& operator=(const ASocket&);
};

#endif
