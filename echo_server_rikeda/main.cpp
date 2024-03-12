#include <string.h>

#include <iostream>

#include "EchoServer.hpp"
#include "EventHandler.hpp"

int main(void) {
  try {
    EchoServer server("127.0.0.1", 4242);
    EventHandler handler(10);
    handler.startUpHandleServer(server);
  } catch (EchoServer::ServerInternalError& e) {
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
