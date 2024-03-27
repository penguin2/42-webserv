#include "Connection.hpp"

#include <sys/socket.h>

#include "EventManager.hpp"
#include "Server.hpp"

char Connection::recv_buffer_[Connection::kRecvBufferSize];

Connection::Connection(int socket_fd)
    : ASocket(socket_fd), state_(Connection::kRecv) {}

Connection::~Connection() {}

int Connection::handler(Server* server, EventManager* event_manager) {
  switch (state_) {
    case Connection::kRecv:
      return handlerRecv(server, event_manager);
    case Connection::kSend:
      return handlerSend(server, event_manager);
    case Connection::kClosed:
    default:
      break;
  }
  return 0;
}

int Connection::errorHandler(Server* server) {
  setState(Connection::kClosed);
  return server->closeSocket(this);
}

void Connection::setState(State state) { state_ = state; }

bool Connection::isReadyResponse() {
  const std::string::size_type new_line_index = raw_request_.find('\n');

  if (new_line_index != std::string::npos) {
    const std::string::size_type new_response_size = new_line_index + 1;
    response_sent_size_ = 0;
    raw_response_ = std::string("ECHO/1.0 200 OK\n") +
                   raw_request_.substr(0, new_response_size);
    raw_request_.erase(0, new_response_size);
    return true;
  }
  return false;
}

int Connection::handlerRecv(Server* server, EventManager* event_manager) {
  const int recv_size = recv(socket_fd_, Connection::recv_buffer_,
                             Connection::kRecvBufferSize, 0);
  if (recv_size > 0) {
    raw_request_.append(Connection::recv_buffer_, recv_size);
    return 0;
  }
  if (recv_size == 0) {
    setState(Connection::kClosed);
    server->closeSocket(this);
    return -1;
  }
  if (isReadyResponse()) {
    setState(Connection::kSend);
    event_manager->modify(socket_fd_, this, EventManager::kWrite);
  }
  return -1;
}

int Connection::handlerSend(Server* server, EventManager* event_manager) {
  if (response_sent_size_ == raw_response_.size()) {
    if (isReadyResponse()) return 0;
    setState(Connection::kRecv);
    event_manager->modify(socket_fd_, this, EventManager::kRead);
    return -1;
  }

  const int send_size =
      send(socket_fd_, raw_response_.c_str() + response_sent_size_,
           raw_response_.size() - response_sent_size_, 0);
  if (send_size > 0) {
    response_sent_size_ += send_size;
    return 0;
  }
  setState(Connection::kClosed);
  server->closeSocket(this);
  return -1;
}
