#ifndef WEBSERV_CONNECTION_H
#define WEBSERV_CONNECTION_H

#include <string>

#include "ASocket.hpp"

class Connection : public ASocket {
 public:
  Connection(int socket_fd);
  ~Connection();

  int handler(Server* server, EventManager* event_manager);
  int errorHandler(Server* server);

 private:
  typedef enum eState { kRecv, kSend, kClosed } State;

  State state_;

  std::string raw_request_;
  std::string raw_response_;
  std::string::size_type response_sent_size_;

  // TODO: http handling
  // TODO: cgi handling

  static const int kRecvBufferSize = 1024;
  static char recv_buffer_[kRecvBufferSize];

  Connection();
  Connection(const Connection&);
  Connection& operator=(const Connection&);

  void setState(State state);

  bool isReadyResponse();

  int handlerRecv(Server* server, EventManager* event_manager);
  int handlerSend(Server* server, EventManager* event_manager);
};

#endif
