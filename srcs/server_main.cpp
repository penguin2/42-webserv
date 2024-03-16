#include <string.h>

#include <cerrno>
#include <iostream>

#include "Server.hpp"
#include "ServerHandler.hpp"

int main(void) {
  try {
    Server server(SERVER_HOST, SERVER_PORT);
    ServerHandler handler(10);
    handler.startUpHandle(server);
  } catch (Server::ServerInternalError& e) {
    std::cerr << "Server Error" << std::endl;
    return 1;
    strerror(errno);
  } catch (ServerHandler::HandlerError& e) {
    std::cerr << "Handler Error" << std::endl;
    std::cerr << strerror(errno) << std::endl;
    strerror(errno);
    return 1;
  }

  return 0;
}
