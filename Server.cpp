//
// Created by daimiaopeng on 2019/10/31.
//

#include "Server.h"

using namespace std;
using namespace std::placeholders;

void Server::run() {
    initsocket();

    struct epoll_event events[20];

    for (;;) {
        int nums_epoll = epoll_wait(epoll_fd, events, 128, -1);
        if (nums_epoll == -1) {
            perror("epoll_wait() error");
        }
        for (int i = 0; i < nums_epoll; i++) {
            if (!(events[i].events & EPOLLIN)) {
                continue;
            }
            event_infor *infor = static_cast<event_infor *>(events[i].data.ptr);
            infor->eventCallback(infor);
        }
    }
}

void Server::eventset() {

}

void Server::eventadd(int events, event_infor &infor) {
    epoll_event event{0, {0}};
    event.events = events;
    infor.events = events;
    event.data.ptr = static_cast<void *>(&infor);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infor.fd, &event);
}

void Server::initsocket() {
    event_infor *l_infor = new event_infor;
    l_infor->fd = lfd;
    l_infor->ip = inet_ntoa(serv_addr.sin_addr);
    l_infor->port = _port;
    l_infor->eventCallback = [&](event_infor *infor) { acceptconn(infor); };
    eventadd(EPOLLIN, *l_infor);

}

void Server::acceptconn(event_infor *infor) {
    //有请求时 cfd的Callback()调用acceptconn(),accept()一个客户端,然后添加到epoll
    //设置Callback为自定义的read函数
//    初始化一个socket结构
    sockaddr_in socket_addr{};
    socklen_t client_addr_len = sizeof(socket_addr);
    int cfd = accept(infor->fd, (struct sockaddr *) &socket_addr, &client_addr_len);
    if (cfd == -1) {
        if (errno != EAGAIN && errno != EINTR) {
            sleep(1);
        }
        printf("%s:accept,%s\n", __func__, strerror(errno));
        return;
    }
    event_infor *cli_event_infor = new event_infor;
    cli_event_infor->fd = cfd;
    cli_event_infor->ip = inet_ntoa(socket_addr.sin_addr);
    cli_event_infor->eventCallback = [&](event_infor *infor) { recvdata(infor); };
    eventadd(EPOLLIN, *cli_event_infor);
}

void Server::recvdata(event_infor *infor) {
    char buff[BUFF_MAX]{};
//    read(infor1->fd, buff,BUFF_MAX);
    int n = 0;
    while ((n = read(infor->fd, buff, BUFF_MAX)) > 1) {
        printf("%s 发来一条消息: %s\n", infor->ip.c_str(), buff);
        bzero(buff, sizeof(buff));
    }
    //如果退出的话要delete
    delete(infor);
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, infor->fd, nullptr);
    close(infor->fd);

}

void Server::test() {
    cout << "test " << this->lfd << endl;
}

