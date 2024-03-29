#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <config_file>" << std::endl;
    return EXIT_FAILURE;
  }
  if (std::signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    LOG(ERROR, "signal: ", std::strerror(errno));
    return EXIT_FAILURE;
  }

  Server server(argv[1]);

  if (server.start() < 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
