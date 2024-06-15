#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"

static void printUsage(char** argv) {
  if (argv != NULL && argv[0] != NULL)
    std::cerr << "usage: " << argv[0] << " <config_file>" << std::endl;
}

static int setSignalHandlers() {
  if (std::signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    LOG(ERROR, "signal: ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printUsage(argv);
    return EXIT_FAILURE;
  }

  Logger::getInstance().init();

  if (setSignalHandlers() < 0) return EXIT_FAILURE;

  Server server(argv[1]);
  if (server.start() < 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
