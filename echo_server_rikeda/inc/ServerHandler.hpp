#ifndef Wevserv_ServerHandler_H_
#define Wevserv_ServerHandler_H_

#include "AHandler.hpp"
#include "Server.hpp"

class ServerHandler : public AHandler {
 public:
  ServerHandler(size_t max_event_size);
  ~ServerHandler(void);

  void startUpHandle(Server server);
};

#endif
