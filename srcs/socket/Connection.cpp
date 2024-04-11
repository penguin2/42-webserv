#include "Connection.hpp"

#include <sys/socket.h>

#include <string>

#include "EventManager.hpp"
#include "Server.hpp"

char Connection::recv_buffer_[Connection::kRecvBufferSize];

Connection::Connection(int socket_fd)
    : ASocket(socket_fd), state_(connection::RECV) {}

Connection::~Connection() {}

int Connection::handler(Server* server, EventManager* event_manager) {
  int status;
  switch (state_) {
    case connection::RECV:
      status = handlerRecv(event_manager);
      break;
    case connection::SEND:
      status = handlerSend(event_manager);
      break;
    case connection::CLOSED:
    default:
      status = -1;
      break;
  }
  server->updateTimeout(this);
  return status;
}

int Connection::handlerRecv(EventManager* event_manager) {
  const int recv_size = recv(socket_fd_, Connection::recv_buffer_,
                             Connection::kRecvBufferSize, 0);
  if (recv_size <= 0) {
    updateState(connection::CLOSED, event_manager);
    return -1;
  }
  http_mock_.appendClientData(std::string(Connection::recv_buffer_, recv_size));
  const connection::State next_state = http_mock_.httpHandler();
  if (updateState(next_state, event_manager) < 0) return -1;
  return 0;
}

int Connection::handlerSend(EventManager* event_manager) {
  if (response_.empty()) {
    response_ = http_mock_.getResponse();
    response_sent_size_ = 0;
  }
  const int send_size =
      send(socket_fd_, response_.c_str() + response_sent_size_,
           response_.size() - response_sent_size_, 0);
  if (send_size <= 0) {
    updateState(connection::CLOSED, event_manager);
    return -1;
  }
  response_sent_size_ += send_size;
  if (response_sent_size_ == response_.size()) {
    response_.clear();
    response_sent_size_ = 0;
    const connection::State next_state = http_mock_.httpHandler();
    if (updateState(next_state, event_manager) < 0) return -1;
  }
  return 0;
}

int Connection::updateState(connection::State new_state,
                            EventManager* event_manager) {
  if (state_ == new_state) return 0;

  const connection::State prev_state = state_;
  state_ = new_state;

  switch (new_state) {
    case connection::RECV:
      if (prev_state == connection::SEND)
        return event_manager->modify(socket_fd_, this, EventManager::kRead);
      break;
    case connection::SEND:
      if (prev_state == connection::RECV)
        return event_manager->modify(socket_fd_, this, EventManager::kWrite);
      break;
    case connection::CLOSED:
    default:
      break;
  }
  return 0;
}
