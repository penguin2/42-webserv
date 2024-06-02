#ifndef WEBSERV_CONNECTION_H
#define WEBSERV_CONNECTION_H

#include <ostream>
#include <string>

#include "ASocket.hpp"
#include "Cgi.hpp"
#include "ConnectionState.hpp"
#include "EventManager.hpp"
#include "SocketAddress.hpp"
#include "TimeoutManager.hpp"
#include "config/ServerConfig.hpp"

#ifdef MOCK
#include "HttpMock.hpp"
#else
#include "Http.hpp"
#endif

class Connection : public ASocket {
 public:
  Connection(int socket_fd, const SocketAddress& local_address,
             const SocketAddress& peer_address,
             const std::vector<const ServerConfig*>& server_configs,
             EventManager* event_manager, TimeoutManager* timeout_manager);
  ~Connection();

  int handler(Server* server);
  int handlerTimeout();

  SocketAddress getPeerAddress() const;

  static void initTransitHandlers();

 private:
  SocketAddress peer_address_;
  connection::State state_;
  Http http_;
  EventManager* event_manager_;
  TimeoutManager* timeout_manager_;

  std::string response_;
  std::size_t response_sent_size_;

  Cgi* cgi_;

  static const int kRecvBufferSize = 1024;
  static char recv_buffer_[kRecvBufferSize];

  int handlerRecv();
  int handlerSend();
  int handlerCgi();
  int handlerCgiRead();
  int handlerCgiWrite();

  void clearCgi();

  int updateState(connection::State new_state);

  Connection();
  Connection(const Connection&);
  Connection& operator=(const Connection&);

  typedef std::pair<connection::State, connection::State> StatePair;
  typedef int (*TransitHandler)(Connection*);
  typedef std::map<StatePair, TransitHandler> TransitHandlerMap;

  static TransitHandlerMap transitHandlers;

  static int recvToSend(Connection* conn);
  static int recvToCgi(Connection* conn);
  static int sendToRecv(Connection* conn);
  static int cgiToSend(Connection* conn);
};

std::ostream& operator<<(std::ostream& os, const Connection& connection);

#endif
