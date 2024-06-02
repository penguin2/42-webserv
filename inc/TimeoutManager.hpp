#ifndef WEBSERV_TIMEOUT_MANAGER_H
#define WEBSERV_TIMEOUT_MANAGER_H

#include <map>
#include <vector>

#include "Time.hpp"

class ASocket;

class TimeoutManager {
 public:
  TimeoutManager();
  ~TimeoutManager();

  std::vector<ASocket*> findTimeouts() const;

  int insert(ASocket* socket, const Time& limit);
  int update(ASocket* socket, const Time& new_limit);
  int erase(ASocket* socket);

  static const Time kDefaultTimeoutLimit;
  static const Time kCgiTimeoutLimit;

 private:
  std::multimap<Time, ASocket*> timeout_logger_;

  TimeoutManager(const TimeoutManager&);
  TimeoutManager& operator=(const TimeoutManager&);
};

#endif
