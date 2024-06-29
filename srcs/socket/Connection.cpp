#include "Connection.hpp"

#include <sys/socket.h>

#include <string>
#include <utility>

#include "EventManager.hpp"
#include "Logger.hpp"

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
  connection::State next_state;

  switch (state_) {
    case connection::RECV:
      next_state = http_.httpHandler(connection::RECV_TIMEOUT);
      break;
    case connection::SEND:
      next_state = connection::CLOSED;
      break;
    case connection::CGI:
      next_state = http_.httpHandler(connection::CGI_TIMEOUT);
      break;
    default:
      next_state = connection::CLOSED;
      break;
  }

  if (updateState(next_state) < 0) return -1;
  return 0;
}

SocketAddress Connection::getPeerAddress() const { return peer_address_; }

int Connection::handlerRecv() {
  const ssize_t recv_size = recv(socket_fd_, Connection::recv_buffer_,
                                 Connection::kRecvBufferSize, 0);
  unsetEventType(EventType::READ);
  if (recv_size <= 0) {
    updateState(connection::CLOSED);
    return -1;
  }
  http_.appendClientData(std::string(Connection::recv_buffer_, recv_size));
  const connection::State next_state = http_.httpHandler(state_);
  if (updateState(next_state) < 0) return -1;
  return 0;
}

int Connection::handlerSend() {
  if (response_.empty()) {
    response_ = http_.getResponse();
    response_sent_size_ = 0;
  }
  const ssize_t send_size =
      send(socket_fd_, response_.c_str() + response_sent_size_,
           response_.size() - response_sent_size_, 0);
  unsetEventType(EventType::WRITE);
  if (send_size <= 0) {
    updateState(connection::CLOSED);
    return -1;
  }
  response_sent_size_ += send_size;
  if (response_sent_size_ == response_.size()) {
    response_.clear();
    response_sent_size_ = 0;
    Logger::getInstance().log(Logger::INFO, Connection::makeLog());
    const connection::State next_state = http_.httpHandler(state_);
    if (updateState(next_state) < 0) return -1;
  }
  return 0;
}

int Connection::handlerCgi() {
  const bool is_write_event = isEventType(EventType::WRITE);
  const bool is_read_event = isEventType(EventType::READ);

  if ((is_write_event && handlerCgiWrite() < 0) ||
      (is_read_event && handlerCgiRead() < 0))
    return -1;
  return 0;
}

int Connection::handlerCgiDone() {
  int exit_status;
  const pid_t exited_pid = cgi_->tryWaitNoHang(&exit_status);
  if (exited_pid == 0) return 0;

  if (event_manager_->erase(cgi_->getReadFd(), this, EventType::READ) < 0 ||
      cgi_->clearReadFd() < 0)
    return -1;

  const bool is_good_exit_status = (exited_pid > 0 && exit_status == 0);

  const connection::State current_state =
      is_good_exit_status ? state_ : connection::CGI_ERROR;
  if (is_good_exit_status)
    http_.setCgiResponseMessage(cgi_->getCgiResponseMessage());

  const connection::State next_state = http_.httpHandler(current_state);
  if (updateState(next_state) < 0) return -1;

  return 0;
}

int Connection::handlerCgiRead() {
  unsetEventType(EventType::READ);
  if (cgi_->isReadDone()) return handlerCgiDone();
  if (cgi_->readMessage() < 0) return -1;
  return 0;
}

int Connection::handlerCgiWrite() {
  unsetEventType(EventType::WRITE);
  const bool is_write_done = cgi_->writeMessage() < 0 || cgi_->isWriteDone();
  if (is_write_done &&
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
  const connection::State prev_state = state_;
  state_ = new_state;

  if (new_state == connection::CLOSED ||
      updateTimeout(prev_state, new_state) < 0)
    return -1;

  if (prev_state == new_state) return 0;
  Connection::TransitHandlerMap::const_iterator it =
      Connection::transitHandlers.find(std::make_pair(prev_state, new_state));
  if (it == Connection::transitHandlers.end()) return -1;
  return it->second(this);
}

int Connection::updateTimeout(connection::State prev_state,
                              connection::State new_state) {
  Time new_timeout_limit = Time(0, 0);
  switch (new_state) {
    case connection::RECV:
      if (prev_state != connection::RECV)
        new_timeout_limit = TimeoutManager::kDefaultTimeoutLimit;
      break;
    case connection::SEND:
      new_timeout_limit = TimeoutManager::kDefaultTimeoutLimit;
      break;
    case connection::CGI:
      new_timeout_limit = TimeoutManager::kCgiTimeoutLimit;
      break;
    default:
      break;
  }
  if (new_timeout_limit > Time(0, 0) &&
      timeout_manager_->update(this, new_timeout_limit) < 0)
    return -1;
  return 0;
}

std::string Connection::makeLog() const {
  return getLocalAddress().toString() + " -> " + getPeerAddress().toString() +
         " " + http_.makeResponseLog();
}

Connection::TransitHandlerMap Connection::transitHandlers;

void Connection::initTransitHandlers() {
  Connection::transitHandlers[std::make_pair(
      connection::RECV, connection::SEND)] = Connection::recvToSend;
  Connection::transitHandlers[std::make_pair(
      connection::RECV, connection::CGI)] = Connection::recvToCgi;
  Connection::transitHandlers[std::make_pair(
      connection::SEND, connection::CGI)] = Connection::sendToCgi;
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
  return 0;
}

int Connection::sendToCgi(Connection* conn) {
  conn->cgi_ = Cgi::createCgi(conn->http_.getCgiRequest());
  if (conn->cgi_ == NULL ||
      conn->event_manager_->erase(conn->socket_fd_, conn, EventType::WRITE) <
          0 ||
      conn->event_manager_->insert(conn->cgi_->getReadFd(), conn,
                                   EventType::READ) < 0 ||
      conn->event_manager_->insert(conn->cgi_->getWriteFd(), conn,
                                   EventType::WRITE) < 0)
    return -1;
  return 0;
}

int Connection::sendToRecv(Connection* conn) {
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
