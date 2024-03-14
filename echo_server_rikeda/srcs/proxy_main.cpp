#include <string.h>

#include <cerrno>
#include <iostream>

#include "EventHandler.hpp"
#include "ProxyHandler.hpp"
#include "Server.hpp"

#define PROXY_HOST "127.0.0.1"
#define PROXY_PORT 4242

int main(void) {
  try {
    Server server(PROXY_HOST, PROXY_PORT);
    ProxyHandler handler(10);
    handler.startUpHandleProxy(server);
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
