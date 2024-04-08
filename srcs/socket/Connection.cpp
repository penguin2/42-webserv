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
  const connection::State prev_state = state_;

  if (prev_state == new_state) return 0;

  state_ = new_state;

  if (new_state == connection::CLOSED) return 0;

  switch (prev_state) {
    case connection::RECV:
      return updateStateFromRecv(new_state, event_manager);
    case connection::SEND:
      return updateStateFromSend(new_state, event_manager);
    default:
      break;
  }
  return 0;
}

int Connection::updateStateFromRecv(connection::State new_state,
                                    EventManager* event_manager) {
  switch (new_state) {
    case connection::RECV:
      break;
    case connection::SEND:
      return event_manager->modify(socket_fd_, this, EventManager::kWrite);
    default:
      break;
  }
  return 0;
}

int Connection::updateStateFromSend(connection::State new_state,
                                    EventManager* event_manager) {
  switch (new_state) {
    case connection::RECV:
      return event_manager->modify(socket_fd_, this, EventManager::kRead);
    case connection::SEND:
      break;
    default:
      break;
  }
  return 0;
}
