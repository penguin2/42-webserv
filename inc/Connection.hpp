#ifndef WEBSERV_CONNECTION_H
#define WEBSERV_CONNECTION_H

#include "ASocket.hpp"

class EventManager;

class Connection : ASocket {
 private:
  typedef enum State {
    RECV,
    SEND,
    CLOSED,
  } t_state;

  t_state state_;
  // TODO: unnecessary?!
  EventManager* event_manager_;

  // TODO: make class for timer?!
  // int timeout_;
  // int timeout_header_;

  // TODO: http handling
  // TODO: cgi handling

 public:
  Connection(int socket_fd, EventManager* event_manager);
  ~Connection();

  int handler();

 private:
  Connection();
  Connection(const Connection&);
  Connection& operator=(const Connection&);

  int handlerRecv();
  int handlerSend();
};

#endif
