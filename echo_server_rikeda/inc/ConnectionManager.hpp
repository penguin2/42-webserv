#ifndef Webserv_ConnectionManager_H_
#define Webserv_ConnectionManager_H_

#include <vector>

#include "Connection.hpp"

class ConnectionManager {
 public:
  ConnectionManager(void);
  ~ConnectionManager(void);

  void add(int up_stream_fd, int down_stream_fd,
           Connection::t_connection_type type);
  int del(int fd);
  Connection* searchFromUpStreamFds(int fd);
  Connection* searchFromDownStreamFds(int fd);

 private:
  std::vector<Connection*> connections;
};

#endif
