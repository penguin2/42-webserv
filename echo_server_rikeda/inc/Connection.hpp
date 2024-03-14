#include <string>

class Connection {
 public:
	 Connection(int up_stream_fd, int down_stream_fd);
	 ~Connection(void);

	 typedef enum e_connection_type {
		 CLIENT_SERVER,
		 SERVER_SERVER,
	 }	t_connection_type;
	 void	recvToBuffer(void);
	 void sendBufferContents(void);

 private:
  static const size_t BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  int up_stream_fd_;
  int down_stream_fd_;
  t_connection_type type;
};
