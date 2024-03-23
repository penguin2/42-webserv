#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include <map>
#include <string>
#include <vector>

#include "Config.hpp"
#include "Connection.hpp"
#include "EventManager.hpp"
#include "ListenSocket.hpp"

class Server {
 private:
  const Config config_;
  const EventManager event_manager_;
  std::map<int, Connection*> connections_;
  std::vector<ListenSocket*> listen_sockets_;

 public:
  Server(const char* config_file);
  ~Server();

  int addConnection(int socket_fd);
  int closeConnection(Connection* connection);
  int checkTimeouts();
  int filterConnections();

  int start();
  int loop();

 private:
  Server();
  Server(const Server&);
  Server& operator=(const Server&);
};

#endif
