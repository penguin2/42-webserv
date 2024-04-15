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

  int insert(ASocket* socket);
  int update(ASocket* socket);
  int erase(ASocket* socket);

 private:
  std::multimap<Time, ASocket*> timeout_logger_;

  static const Time kDefaultTimeoutLimit;

  TimeoutManager(const TimeoutManager&);
  TimeoutManager& operator=(const TimeoutManager&);
};

#endif
