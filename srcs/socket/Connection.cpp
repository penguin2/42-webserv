#include "Connection.hpp"

#include <sys/socket.h>

#include <string>
#include <utility>

#include "EventManager.hpp"

char Connection::recv_buffer_[Connection::kRecvBufferSize];

Connection::Connection(int socket_fd, const SocketAddress& local_address,
                       const SocketAddress& peer_address,
                       const std::vector<const ServerConfig*>& server_configs,
                       EventManager* event_manager,
                       TimeoutManager* timeout_manager)
    : ASocket(socket_fd, local_address),
      peer_address_(peer_address),
      state_(connection::RECV),
      http_(peer_address, server_configs),
      event_manager_(event_manager),
      timeout_manager_(timeout_manager),
      cgi_(NULL) {}

Connection::~Connection() { clearCgi(); }

int Connection::handler(Server* server) {
  int status;
  switch (state_) {
    case connection::RECV:
      status = handlerRecv();
      break;
    case connection::SEND:
      status = handlerSend();
      break;
    case connection::CGI:
      status = handlerCgi();
      break;
    case connection::CLOSED:
    default:
      status = -1;
      break;
  }
  return status;

  (void)server;
}

int Connection::handlerTimeout() {
  if (state_ == connection::SEND) return 0;

  connection::State next_state = connection::CLOSED;

  if (state_ == connection::CGI)
    next_state = http_.httpHandler(connection::CGI_TIMEOUT);

  if (updateState(next_state) < 0) return -1;
  return 0;
}

SocketAddress Connection::getPeerAddress() const { return peer_address_; }

int Connection::handlerRecv() {
  const int recv_size = recv(socket_fd_, Connection::recv_buffer_,
                             Connection::kRecvBufferSize, 0);
  if (recv_size <= 0) {
    updateState(connection::CLOSED);
    return -1;
  }
  http_.appendClientData(std::string(Connection::recv_buffer_, recv_size));
  const connection::State next_state = http_.httpHandler(this->state_);
  if (updateState(next_state) < 0) return -1;
  return 0;
}

int Connection::handlerSend() {
  if (response_.empty()) {
    response_ = http_.getResponse();
    response_sent_size_ = 0;
  }
  const int send_size =
      send(socket_fd_, response_.c_str() + response_sent_size_,
           response_.size() - response_sent_size_, 0);
  if (send_size <= 0) {
    updateState(connection::CLOSED);
    return -1;
  }
  response_sent_size_ += send_size;
  if (response_sent_size_ == response_.size()) {
    response_.clear();
    response_sent_size_ = 0;
    const connection::State next_state = http_.httpHandler(this->state_);
    if (updateState(next_state) < 0) return -1;
  }
  return 0;
}

int Connection::handlerCgi() {
  const bool is_write_event = EventType::isWrite(getEventType());
  const bool is_read_event = EventType::isRead(getEventType());

  if (is_write_event && handlerCgiWrite() < 0) return -1;
  if (is_read_event && handlerCgiRead() < 0) return -1;

  return 0;
}

int Connection::handlerCgiRead() {
  if (cgi_->readMessage() < 0) return -1;
  if (cgi_->isReadDone()) {
    if (event_manager_->erase(cgi_->getReadFd(), this, EventType::READ) < 0 ||
        cgi_->clearReadFd() < 0)
      return -1;
    http_.setCgiResponseMessage(cgi_->getCgiResponseMessage());
    const connection::State current_state =
        cgi_->clearProcess() == 0 ? this->state_ : connection::CGI_ERROR;
    const connection::State next_state = http_.httpHandler(current_state);
    if (updateState(next_state) < 0) return -1;
  }
  return 0;
}

int Connection::handlerCgiWrite() {
  if (cgi_->writeMessage() < 0) return -1;
  if (cgi_->isWriteDone() &&
      (event_manager_->erase(cgi_->getWriteFd(), this, EventType::WRITE) < 0 ||
       cgi_->clearWriteFd() < 0))
    return -1;
  return 0;
}

void Connection::clearCgi() {
  if (cgi_ != NULL) {
    if (cgi_->getReadFd() != -1)
      event_manager_->erase(cgi_->getReadFd(), this, EventType::READ);
    if (cgi_->getWriteFd() != -1)
      event_manager_->erase(cgi_->getWriteFd(), this, EventType::WRITE);
    delete cgi_;
    cgi_ = NULL;
  }
}

int Connection::updateState(connection::State new_state) {
  if (state_ == new_state) return 0;

  const connection::State prev_state = state_;
  state_ = new_state;

  Connection::TransitHandlerMap::const_iterator it =
      Connection::transitHandlers.find(std::make_pair(prev_state, new_state));
  if (it == Connection::transitHandlers.end()) return -1;
  return it->second(this);
}

Connection::TransitHandlerMap Connection::transitHandlers;

void Connection::initTransitHandlers() {
  Connection::transitHandlers[std::make_pair(
      connection::RECV, connection::SEND)] = Connection::recvToSend;
  Connection::transitHandlers[std::make_pair(
      connection::RECV, connection::CGI)] = Connection::recvToCgi;
  Connection::transitHandlers[std::make_pair(
      connection::SEND, connection::RECV)] = Connection::sendToRecv;
  Connection::transitHandlers[std::make_pair(
      connection::CGI, connection::SEND)] = Connection::cgiToSend;
}

int Connection::recvToSend(Connection* conn) {
  return conn->event_manager_->modify(conn->socket_fd_, conn, EventType::WRITE);
}

int Connection::recvToCgi(Connection* conn) {
  conn->cgi_ = Cgi::createCgi(conn->http_.getCgiRequest());
  if (conn->cgi_ == NULL ||
      conn->event_manager_->erase(conn->socket_fd_, conn, EventType::READ) <
          0 ||
      conn->event_manager_->insert(conn->cgi_->getReadFd(), conn,
                                   EventType::READ) < 0 ||
      conn->event_manager_->insert(conn->cgi_->getWriteFd(), conn,
                                   EventType::WRITE) < 0)
    return -1;
  conn->timeout_manager_->update(conn, TimeoutManager::kCgiTimeoutLimit);
  return 0;
}

int Connection::sendToRecv(Connection* conn) {
  conn->timeout_manager_->update(conn, TimeoutManager::kDefaultTimeoutLimit);
  return conn->event_manager_->modify(conn->socket_fd_, conn, EventType::READ);
}

int Connection::cgiToSend(Connection* conn) {
  conn->clearCgi();
  return conn->event_manager_->insert(conn->socket_fd_, conn, EventType::WRITE);
}

std::ostream& operator<<(std::ostream& os, const Connection& connection) {
  return os << "fd: " << connection.getSocketFd() << " (local) "
            << connection.getLocalAddress() << " -> (peer) "
            << connection.getPeerAddress();
}
