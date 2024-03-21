#ifndef Webserv_Connection_H_
#define Webserv_Connection_H_
#include <string>

class Connection {
 public:
  Connection(int up_stream_fd, int down_stream_fd);
  ~Connection(void);

  void recvToBuffer(int fd);
  void sendBufferContents(int fd);
  void sendResponse(int fd);
  int getUpStreamFd(void) const;
  int getDownStreamFd(void) const;
  void setUpStreamFd(int fd);
  void setDownStreamFd(int fd);

 private:
  static const size_t BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  int up_stream_fd_;
  int down_stream_fd_;

  Connection(void);
};

int addNonblockingFlag(int fd);

#endif
