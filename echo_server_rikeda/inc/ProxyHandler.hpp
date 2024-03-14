#ifndef Wevserv_ProxyHandler_H_
#define Wevserv_ProxyHandler_H_

#include "AHandler.hpp"
#include "Server.hpp"

class ProxyHandler : public AHandler {
 public:
  ProxyHandler(size_t max_event_size);
  ~ProxyHandler(void);

  void startUpHandle(Server server);
  int connectToUpStreamServer(void);
};

#endif
