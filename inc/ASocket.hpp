#ifndef WEBSERV_ASOCKET_H
#define WEBSERV_ASOCKET_H

class ASocket {
 protected:
  int socket_fd_;

 public:
  virtual int handler() = 0;
  virtual ~ASocket();

 protected:
  ASocket(int socket_fd);

 private:
  ASocket();
  ASocket(const ASocket&);
  ASocket& operator=(const ASocket&);
};

#endif
