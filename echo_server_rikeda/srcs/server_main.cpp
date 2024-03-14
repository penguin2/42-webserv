#include <string.h>

#include <cerrno>
#include <iostream>

#include "EventHandler.hpp"
#include "ProxyHandler.hpp"
#include "Server.hpp"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 4243

int main(void) {
  try {
    Server server(SERVER_HOST, SERVER_PORT);
    EventHandler handler(10);
    handler.startUpHandleServer(server);
  } catch (Server::ServerInternalError& e) {
    std::cerr << "Server Error" << std::endl;
    return 1;
    strerror(errno);
  } catch (EventHandler::EventHandlerError& e) {
    std::cerr << "Handler Error" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    strerror(errno);
    return 1;
  }

  return 0;
}
