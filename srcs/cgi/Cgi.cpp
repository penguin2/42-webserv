#include "Cgi.hpp"

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "Logger.hpp"
#include "Server.hpp"
#include "SysUtils.hpp"

char Cgi::read_buffer_[Cgi::kReadBufferSize];

Cgi::Cgi()
    : read_fd_(-1),
      write_fd_(-1),
      pid_(-1),
      is_receiving_(false),
      cgi_request_message_sent_size_(0) {}

Cgi::~Cgi() {
  clearReadFd();
  clearWriteFd();
  if (pid_ != -1 && kill(pid_, SIGKILL) < 0)
    LOG(WARN, "kill(cgi): ", std::strerror(errno));
  if (waitpid(pid_, NULL, WNOHANG) < 0)
    LOG(WARN, "waitpid(cgi): ", std::strerror(errno));
}

int Cgi::getReadFd() const { return read_fd_; }

int Cgi::getWriteFd() const { return write_fd_; }

int Cgi::clearReadFd() { return SysUtils::clearFd(&read_fd_); }

int Cgi::clearWriteFd() { return SysUtils::clearFd(&write_fd_); }

int Cgi::readMessage() {
  const int read_size = read(read_fd_, Cgi::read_buffer_, Cgi::kReadBufferSize);
  if (read_size < 0) {
    LOG(WARN, "read(cgi): ", std::strerror(errno));
    return -1;
  }
  if (read_size == 0) {
    is_receiving_ = false;
    return 0;
  }
  cgi_response_message_.append(std::string(Cgi::read_buffer_, read_size));
  return 0;
}

int Cgi::writeMessage() {
  const int write_size = write(
      write_fd_, cgi_request_message_.c_str() + cgi_request_message_sent_size_,
      cgi_request_message_.size() - cgi_request_message_sent_size_);
  if (write_size <= 0) {
    LOG(WARN, "write(cgi): ", std::strerror(errno));
    return -1;
  }
  cgi_request_message_sent_size_ += write_size;
  return 0;
}

std::string Cgi::getCgiResponseMessage() const { return cgi_response_message_; }

bool Cgi::isReadDone() const { return !is_receiving_; }

bool Cgi::isWriteDone() const {
  return cgi_request_message_sent_size_ == cgi_request_message_.size();
}

int Cgi::makePipes(int read_pipe[2], int write_pipe[2]) {
  read_pipe[Cgi::READ_END] = -1;
  read_pipe[Cgi::WRITE_END] = -1;
  write_pipe[Cgi::READ_END] = -1;
  write_pipe[Cgi::WRITE_END] = -1;
  if (pipe(read_pipe) < 0 || pipe(write_pipe) < 0 ||
      SysUtils::addNonblockingFlag(read_pipe[Cgi::READ_END]) < 0 ||
      SysUtils::addNonblockingFlag(write_pipe[Cgi::WRITE_END]) < 0 ||
      SysUtils::addCloseOnExecFlag(read_pipe[Cgi::READ_END]) < 0 ||
      SysUtils::addCloseOnExecFlag(read_pipe[Cgi::WRITE_END]) < 0 ||
      SysUtils::addCloseOnExecFlag(write_pipe[Cgi::READ_END]) < 0 ||
      SysUtils::addCloseOnExecFlag(write_pipe[Cgi::WRITE_END]) < 0) {
    clearPipes(read_pipe, write_pipe);
    return -1;
  }
  return 0;
}

void Cgi::clearPipes(int read_pipe[2], int write_pipe[2]) {
  for (int i = 0; i < 2; ++i) {
    SysUtils::clearFd(&read_pipe[i]);
    SysUtils::clearFd(&write_pipe[i]);
  }
}

Cgi* Cgi::createCgi(const CgiRequestMock* cgi_request) {
  if (cgi_request == NULL) return NULL;

  int read_pipe[2];
  int write_pipe[2];
  if (Cgi::makePipes(read_pipe, write_pipe) < 0) return NULL;

  const int cgi_pid = fork();
  if (cgi_pid < 0) {
    clearPipes(read_pipe, write_pipe);
    LOG(WARN, "fork(cgi): ", std::strerror(errno));
    return NULL;
  }

  // child process: cgi script
  if (cgi_pid == 0) {
    if (dup2(write_pipe[Cgi::READ_END], STDIN_FILENO) < 0 ||
        dup2(read_pipe[Cgi::WRITE_END], STDOUT_FILENO) < 0 ||
        close(STDERR_FILENO) < 0 ||
        chdir(cgi_request->getDirectory().c_str()) < 0) {
      LOG(WARN, "cgi script init: ", std::strerror(errno));
      std::exit(EXIT_FAILURE);
    }

    char* filename = SysUtils::convertToCstring(cgi_request->getFilename());
    char* const* envp = SysUtils::convertToEnvp(cgi_request->getEnvVars());
    char* const argv[2] = {filename, NULL};

    execve(filename, argv, envp);

    LOG(WARN, "execve(cgi): ", std::strerror(errno));
    SysUtils::deleteCstringArray(envp);
    SysUtils::deleteCstring(filename);

    std::exit(EXIT_FAILURE);
  }
  //

  SysUtils::clearFd(&read_pipe[Cgi::WRITE_END]);
  SysUtils::clearFd(&write_pipe[Cgi::READ_END]);

  Cgi* new_cgi = new Cgi();
  new_cgi->pid_ = cgi_pid;
  new_cgi->read_fd_ = read_pipe[Cgi::READ_END];
  new_cgi->write_fd_ = write_pipe[Cgi::WRITE_END];
  new_cgi->is_receiving_ = true;
  new_cgi->cgi_request_message_ = cgi_request->getMessage();
  return new_cgi;
}
