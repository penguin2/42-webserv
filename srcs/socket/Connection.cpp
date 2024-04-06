#include "Connection.hpp"

#include <sys/socket.h>

#include "EventManager.hpp"
#include "Server.hpp"

char Connection::recv_buffer_[Connection::kRecvBufferSize];

Connection::Connection(int socket_fd)
    : ASocket(socket_fd), state_(Connection::kRecv) {}

Connection::~Connection() {}

int Connection::handler(Server* server, EventManager* event_manager) {
  int status;
  switch (state_) {
    case Connection::kRecv:
      status = handlerRecv(event_manager);
      break;
    case Connection::kSend:
      status = handlerSend(event_manager);
      break;
    case Connection::kClosed:
    default:
      status = -1;
      break;
  }
  server->updateTimeout(this);
  return status;
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

int Connection::handlerRecv(EventManager* event_manager) {
  const int recv_size = recv(socket_fd_, Connection::recv_buffer_,
                             Connection::kRecvBufferSize, 0);
  if (recv_size <= 0) {
    setState(Connection::kClosed);
    return -1;
  }
  if (recv_size > 0) {
    // TODO: Http will handle the received message
    // (here the code below is temporary)
    raw_request_.append(Connection::recv_buffer_, recv_size);
    if (isReadyResponse()) {
      setState(Connection::kSend);
      event_manager->modify(socket_fd_, this, EventManager::kWrite);
    }
  }
  return 0;
}

int Connection::handlerSend(EventManager* event_manager) {
  const int send_size =
      send(socket_fd_, raw_response_.c_str() + response_sent_size_,
           raw_response_.size() - response_sent_size_, 0);
  if (send_size <= 0) {
    setState(Connection::kClosed);
    return -1;
  }
  if (send_size > 0) {
    response_sent_size_ += send_size;
    // TODO: Http will handle the sent message
    // (here the code below is temporary)
    if (response_sent_size_ == raw_response_.size() &&
        isReadyResponse() == false) {
      setState(Connection::kRecv);
      event_manager->modify(socket_fd_, this, EventManager::kRead);
    }
  }
  return 0;
}
