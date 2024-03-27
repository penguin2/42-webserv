#ifndef WEBSERV_ASOCKET_H
#define WEBSERV_ASOCKET_H

class Server;
class EventManager;

class ASocket {
 public:
  int getSocketFd() const;
  virtual int handler(Server* server, EventManager* event_manager) = 0;
  virtual int errorHandler(Server* server) = 0;
  virtual ~ASocket();

 protected:
  int socket_fd_;
  ASocket(int socket_fd);

 private:
  ASocket();
  ASocket(const ASocket&);
  ASocket& operator=(const ASocket&);
};

#endif
