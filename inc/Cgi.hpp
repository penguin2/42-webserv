#ifndef WEBSERV_CGI_H
#define WEBSERV_CGI_H

#include <sys/types.h>

#include <string>

#include "CgiRequestMock.hpp"

class Cgi {
 public:
  ~Cgi();

  int getReadFd() const;
  int getWriteFd() const;

  int clearReadFd();
  int clearWriteFd();

  int readMessage();
  int writeMessage();

  std::string getCgiResponseMessage() const;

  bool isReadDone() const;
  bool isWriteDone() const;

  static Cgi* createCgi(const CgiRequestMock* cgi_request);

 private:
  int read_fd_;
  int write_fd_;
  pid_t pid_;
  bool is_receiving_;
  std::string cgi_request_message_;
  std::string cgi_response_message_;
  std::size_t cgi_request_message_sent_size_;

  // man pipe: fd[0](read end of the pipe), fd[1](write end of the pipe)
  typedef enum eReadWriteEnd { READ_END = 0, WRITE_END = 1 } ReadWriteEnd;
  static int makePipes(int read_pipe[2], int write_pipe[2]);
  static void clearPipes(int read_pipe[2], int write_pipe[2]);

  static const int kReadBufferSize = 1024;
  static char read_buffer_[kReadBufferSize];

  Cgi();
  Cgi(const Cgi&);
  Cgi& operator=(const Cgi&);
};

#endif
