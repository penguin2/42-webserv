#ifndef Webserv_Connection_H_
#define Webserv_Connection_H_
#include <string>

class Connection {
 public:
  typedef enum e_connection_type {
    CLIENT_SERVER,
    SERVER_SERVER,
  } t_connection_type;

 public:
  Connection(int up_stream_fd, int down_stream_fd, t_connection_type type);
  ~Connection(void);

  void recvToBuffer(int fd);
  void sendBufferContents(int fd);
  void sendResponse(int fd);
  int getUpStreamFd(void) const;
  int getDownStreamFd(void) const;
  t_connection_type getConnectionType(void) const;

 private:
  static const size_t BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  int up_stream_fd_;
  int down_stream_fd_;
  t_connection_type type_;
};

#endif
