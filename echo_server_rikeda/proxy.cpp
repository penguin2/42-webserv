#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define EPWAIT -1
#define SERVER_PORT 4244
#define PROXY_PORT 4243
#define SERVER_ADDR "127.0.0.1"
#define MAX_EVENT_SIZE 10
#define BUF_SIZE 10000
char buf[BUF_SIZE];

static void errExit(void) {
  std::cerr << strerror(errno) << std::endl;
  exit(1);
}

static void proxy_request(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) errExit();

  // sockaddr init
  struct sockaddr_in a_addr;
  memset(&a_addr, 0, sizeof(struct sockaddr_in));
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons((unsigned short)SERVER_PORT);
  a_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  if (connect(server_fd, (struct sockaddr *)&a_addr, sizeof(a_addr)) < 0)
    errExit();
  std::cout << "Connected" << SERVER_ADDR << ":" << SERVER_PORT << std::endl;
  /* memset(buf, 0, BUF_SIZE); */
  send(server_fd, buf, strlen(buf), 0);
  recv(server_fd, buf, BUF_SIZE, 0);
  close(server_fd);
  std::cout << "Close Connected" << std::endl;
}

int main(void) {
  // socket init
  int listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_socket_fd == -1) errExit();

  // sockaddr init
  struct sockaddr_in a_addr;
  memset(&a_addr, 0, sizeof(struct sockaddr_in));
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons((unsigned short)PROXY_PORT);
  a_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  // bind & listen
  if (bind(listen_socket_fd, (struct sockaddr *)&a_addr, sizeof(a_addr)) == -1)
    errExit();
  std::cout << SERVER_ADDR << ":" << PROXY_PORT << std::endl;
  if (listen(listen_socket_fd, 3) == -1) {
    close(listen_socket_fd);
    errExit();
  }

  // epoll init
  struct epoll_event ev, ev_ret[MAX_EVENT_SIZE];
  int epfd = epoll_create(MAX_EVENT_SIZE);
  if (epfd == -1) errExit();
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = listen_socket_fd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket_fd, &ev) != 0) errExit();

  while (true) {
    // epoll_waitで観測されたイベントがev_retに格納される
    // 戻り値に観測されたイベント数が返される
    int nfds = epoll_wait(epfd, ev_ret, MAX_EVENT_SIZE, -1);
    if (nfds < 0) errExit();
    if (nfds == 0) {
      std::cout << "ZeroEvents" << std::endl;
      continue;
    }
    // 観測されたイベント数ぶんループを回し、イベントに対応する処理を行う
    for (int i = 0; i < nfds; i++) {
      if (ev_ret[i].data.fd == listen_socket_fd) {
        // listenしているソケットでのイベントが観測
        // 新しいクライアントが接続したということになる
        int sockfd = accept(listen_socket_fd, NULL, NULL);
        if (sockfd == -1) errExit();
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.fd = sockfd;
        // acceptで作成されたソケットでのイベントを観測するように設定
        if ((epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev)) < 0) errExit();
      } else if (ev_ret[i].events & EPOLLIN) {
        // listenしているソケット以外でイベントが発生
        // acceptで作成されたソケットでのイベントが観測されたということ
        int sockfd = ev_ret[i].data.fd;
        recv(sockfd, buf, BUF_SIZE, 0);
        std::cout << buf << std::endl;
        proxy_request();
        // acceptに対するイベント観測設定を削除する
        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLOUT;
        ev.data.fd = sockfd;
        // acceptで作成されたソケットでのイベントを観測するように設定
        if ((epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev)) < 0) errExit();
      } else {
        int sockfd = ev_ret[i].data.fd;
        send(sockfd, buf, strlen(buf), 0);
        close(sockfd);
        // acceptに対するイベント観測設定を削除する
        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);
      }
    }
  }
  close(listen_socket_fd);
  return 0;
}
