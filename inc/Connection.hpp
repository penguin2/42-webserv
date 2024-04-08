#ifndef WEBSERV_CONNECTION_H
#define WEBSERV_CONNECTION_H

#include <string>

#include "ASocket.hpp"
#include "ConnectionState.hpp"
#include "HttpMock.hpp"

class Connection : public ASocket {
 public:
  Connection(int socket_fd);
  ~Connection();

  int handler(Server* server, EventManager* event_manager);

 private:
  connection::State state_;
  HttpMock http_mock_;

  std::string response_;
  std::string::size_type response_sent_size_;

  // TODO: cgi handling

  static const int kRecvBufferSize = 1024;
  static char recv_buffer_[kRecvBufferSize];

  Connection();
  Connection(const Connection&);
  Connection& operator=(const Connection&);

  int handlerRecv(EventManager* event_manager);
  int handlerSend(EventManager* event_manager);

  int updateState(connection::State new_state, EventManager* event_manager);
  int updateStateFromRecv(connection::State new_state,
                          EventManager* event_manager);
  int updateStateFromSend(connection::State new_state,
                          EventManager* event_manager);
};

#endif
