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
  std::signal(SIGPIPE, SIG_IGN);

  Server server(argv[1]);

  server.start();

  return EXIT_SUCCESS;
}
