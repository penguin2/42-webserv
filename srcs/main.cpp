#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Server.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <config_file>" << std::endl;
    return EXIT_FAILURE;
  }
  if (std::signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    std::cerr << "signal: " << std::strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  Server server(argv[1]);

  server.start();

  return EXIT_SUCCESS;
}
