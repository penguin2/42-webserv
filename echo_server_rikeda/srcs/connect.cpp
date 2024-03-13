#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define MAX_EVENTS 10

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cerr << "Failed to create socket\n";
    return 1;
  }

  // ソケットを非ブロッキングに設定
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  // 接続先のアドレスを設定
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(4242);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  // connectを非ブロッキングで実行
  int result =
      connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

  std::cout << "result = " << result << std::endl;

  if (result == -1 && errno != EINPROGRESS) {
    std::cerr << "Failed to connect\n";
    close(sockfd);
    return 1;
  }

  // epollを作成
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    std::cerr << "Failed to create epoll\n";
    close(sockfd);
    return 1;
  }

  struct epoll_event event;
  event.events =
      EPOLLIN | EPOLLOUT | EPOLLET;  // 読み込み・書き込み・エッジトリガー有効
  event.data.fd = sockfd;

  // epollにソケットを登録
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
    std::cerr << "Failed to add socket to epoll\n";
    close(epoll_fd);
    close(sockfd);
    return 1;
  }

  struct epoll_event events[MAX_EVENTS];
  int num_events;

  // epoll待機ループ
  while (true) {
    num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (num_events == -1) {
      std::cerr << "epoll_wait error\n";
      break;
    }

    for (int i = 0; i < num_events; ++i) {
      if (events[i].events & EPOLLOUT) {
        // 接続が完了したかチェック
        int error = 0;
        socklen_t len = sizeof(error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (error != 0) {
          std::cerr << "Connection error: " << strerror(error) << "\n";
          break;
        }

        std::cout << "Connected successfully\n";

        send(sockfd, "SSS", 3, 0);
      }
    }
  }

  // 後始末
  close(epoll_fd);
  close(sockfd);

  return 0;
}
