#ifndef Webserv_ConnectionManager_H_
#define Webserv_ConnectionManager_H_

#include <vector>

#include "Connection.hpp"

class ConnectionManager {
 public:
  ConnectionManager(void);
  ~ConnectionManager(void);

  void add(int up_stream_fd, int down_stream_fd);
  int del(int fd);
  Connection* searchFromUpStreamFds(int fd) const;
  Connection* searchFromDownStreamFds(int fd) const;

 private:
  std::vector<Connection*> connections;
};

#endif
