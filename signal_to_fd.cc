#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/epoll.h>
#include <unistd.h>


add_fd_event(int epollfd, int fd) {
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }
}

// 测试统一事件源，信号，网络fd通过epoll来触发，LT(水平触发)，ET(边沿触发)
int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("listen error:");
    }
    struct sockaddr_in my_address;
    bzero(&my_address, sizeof(my_address));
    my_address.sin_family = AF_INET;
    my_address.sin_port = htons(8000);
    my_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = bind(listen_fd, (struct sockaddr *)&my_address, sizeof(struct sockaddr));
    if (ret < 0) {
        perror("bind error:");
    }
    ret = listen(listen_fd, 10);
    if (ret < 0) {
        perror("listen error:");
    }

    int epollfd = epoll_create(10);
    if (epollfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    add_fd_event(epollfd, listen_fd);

    // add signal handler.
    int pipefd[2];
    ret = pipe(pipefd);

    if (ret < 0) {
        perror("pipe error:");
    }

    exit(0);
}
