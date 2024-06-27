#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "ASocket.hpp"
#include "Connection.hpp"
#include "EventManager.hpp"
#include "ListenSocket.hpp"
#include "TimeoutManager.hpp"

class Server {
 public:
  Server(const char* config_filename);
  ~Server();

  int acceptListenSocket(const ListenSocket& listen_socket);

  int start();
  int loop();

 private:
  TimeoutManager* timeout_manager_;
  EventManager* event_manager_;
  std::map<int, ASocket*> sockets_;

  Server();
  Server(const Server&);
  Server& operator=(const Server&);

  void setSockets(const std::map<int, ListenSocket*>& listen_sockets);

  int addConnection(int connected_socket_fd, const SocketAddress& local_address,
                    const SocketAddress& peer_address,
                    const std::vector<const ServerConfig*>& server_configs);

  int handleTimeouts(const std::vector<ASocket*>& timeout_sockets,
                     std::set<ASocket*>& closing_sockets);
  int executeEventSockets(const std::set<ASocket*>& event_sockets,
                          std::set<ASocket*>& closing_sockets);
  int closeSocket(ASocket* socket);
  int closeSockets(const std::set<ASocket*>& closing_sockets);

  static const std::string kDefaultConfigFilename;
};

std::ostream& operator<<(std::ostream& os,
                         const std::map<int, ASocket*>& sockets);

#endif
