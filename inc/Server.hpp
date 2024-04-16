#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include <map>
#include <string>
#include <vector>

#include "ASocket.hpp"
#include "EventManager.hpp"
#include "TimeoutManager.hpp"

class Server {
 public:
  Server(const char* config_file);
  ~Server();

  int acceptListenSocket(int listen_socket_fd);
  int updateTimeout(ASocket* socket);

  int start();
  int loop();

 private:
  TimeoutManager* timeout_manager_;
  EventManager* event_manager_;
  std::map<int, ASocket*> sockets_;

  static const int kDefaultListenBacklog = 511;

  Server();
  Server(const Server&);
  Server& operator=(const Server&);

  int addConnection(int connected_socket_fd);

  int executeEventSockets(const std::vector<ASocket*>& event_sockets,
                          std::vector<ASocket*>& closing_sockets);
  int closeSocket(ASocket* socket);
  int closeSockets(const std::vector<ASocket*>& closing_sockets);

  // TODO: delete test initializing listen_sockets_
  static int testInitListenSockets(std::map<int, ASocket*>& sockets);
};

#endif
