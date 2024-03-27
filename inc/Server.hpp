#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include <deque>
#include <map>
#include <string>

#include "ASocket.hpp"
#include "EventManager.hpp"

class Server {
 public:
  Server(const char* config_file);
  ~Server();

  int acceptListenSocket(int listen_socket_fd);
  int closeSocket(ASocket* socket);

  // TODO: timeout handling
  // int checkTimeouts();

  int start();
  int loop();

 private:
  EventManager* event_manager_;
  std::map<int, ASocket*> sockets_;

  static const int kDefaultListenBacklog = 511;

  Server();
  Server(const Server&);
  Server& operator=(const Server&);

  int addConnection(int connected_socket_fd);

  int executeEventsErrorQueue(std::deque<ASocket*>& errors);
  int executeEventsQueue(std::deque<ASocket*>& events);

  static int makeListenSocket(const std::string& port);
  static int addNonblockingFlag(int fd);

  // TODO: delete test initializing listen_sockets_
  static int testInitListenSockets(std::map<int, ASocket*>& sockets);
};

#endif
