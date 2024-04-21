#ifndef WEBSERV_CONNECTION_H
#define WEBSERV_CONNECTION_H

#include <string>

#include "ASocket.hpp"
#include "Cgi.hpp"
#include "ConnectionState.hpp"
#include "EventManager.hpp"

#ifdef MOCK
#include "HttpMock.hpp"
#else
#include "Http.hpp"
#endif

class Connection : public ASocket {
 public:
  Connection(int socket_fd, EventManager* event_manager);
  ~Connection();

  int handler(Server* server);

  static void initTransitHandlers();

 private:
  connection::State state_;
  Http http_;
  EventManager* event_manager_;

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

#endif
