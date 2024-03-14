#include <string.h>

#include <cerrno>
#include <iostream>

#include "EventHandler.hpp"
#include "Server.hpp"

int main(void) {
  try {
    Server server("127.0.0.1", 4242);
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
