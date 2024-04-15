#include "TimeoutManager.hpp"

#include "ASocket.hpp"
#include "Logger.hpp"

TimeoutManager::TimeoutManager() {}

TimeoutManager::~TimeoutManager() {}

std::vector<ASocket*> TimeoutManager::findTimeouts() const {
  std::vector<ASocket*> timeout_sockets;
  const Time current_time = Time::getCurrentClockTime();

  for (std::multimap<Time, ASocket*>::const_iterator it =
           timeout_logger_.begin();
       it != timeout_logger_.end(); ++it) {
    if (it->first >= current_time) break;
    timeout_sockets.push_back(it->second);
    LOG(INFO, "timeout: fd: ", it->second->getSocketFd());
  }

  return timeout_sockets;
}

int TimeoutManager::insert(ASocket* socket) {
  const std::pair<Time, ASocket*> new_element(
      Time::getCurrentClockTime() + TimeoutManager::kDefaultTimeoutLimit,
      socket);
  socket->setTimeout(new_element.first);
  timeout_logger_.insert(new_element);
  return 0;
}

int TimeoutManager::update(ASocket* socket) {
  if (erase(socket) < 0) return -1;
  return insert(socket);
}

int TimeoutManager::erase(ASocket* socket) {
  const std::pair<std::multimap<Time, ASocket*>::iterator,
                  std::multimap<Time, ASocket*>::iterator>
      found_range = timeout_logger_.equal_range(socket->getTimeout());
  for (std::multimap<Time, ASocket*>::iterator it = found_range.first;
       it != found_range.second; ++it) {
    if (socket == it->second) {
      timeout_logger_.erase(it);
      return 0;
    }
  }
  return -1;
}

// TODO: set default timeout limit
const Time TimeoutManager::kDefaultTimeoutLimit = Time(5, 0);
