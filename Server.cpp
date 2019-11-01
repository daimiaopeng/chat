//
// Created by daimiaopeng on 2019/10/31.
//

#include "Server.h"
#include <iostream>
#include <sys/epoll.h>

using namespace std;

struct event_infor{
    string ip;
    u_int port;
    int fd;
};

void Server::run() {
    char buff[BUFF_MAX];
    struct epoll_event listen_event;
    listen_event.events = EPOLLIN;

    event_infor server_infor;
    server_infor.fd = lfd;
    listen_event.data.ptr = &server_infor;
    int epoll_fd = epoll_create(128);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, lfd, &listen_event);
    struct epoll_event events[20];
    for (;;) {
        int nums_epoll = epoll_wait(epoll_fd, events, 128, NULL);
        if (nums_epoll == -1) {
            perror("epoll_wait() error");
        }
        for (int i = 0; i < nums_epoll; i++) {
            if (!(events[i].events & EPOLLIN)) {
                continue;
            }
            event_infor *infor = static_cast<event_infor *>(events[i].data.ptr);
            if (infor->fd == lfd) {
                eventadd(lfd,epoll_fd);
            } else {
                int client_fd = infor->fd;
                bzero(&buff, sizeof(buff));
                recv(client_fd, &buff, sizeof(buff), 0);
                printf("%s发来一条消息: %s\n",infor->ip.c_str(), buff);

                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                close(client_fd);

            }
        }
    }
}

void Server::eventset() {

}

void Server::eventadd(int lfd,int epoll_fd) {
    sockaddr_in socket_addr;
    bzero(&socket_addr, sizeof(socket_addr));
    socklen_t client_addr_len = sizeof(socket_addr);
    int cfd = accept(lfd, (struct sockaddr *) &socket_addr, &client_addr_len);

    epoll_event client_event;
    event_infor client_infot;
    client_infot.fd = cfd;
    client_infot.ip = inet_ntoa(socket_addr.sin_addr);
    client_event.data.ptr = static_cast<void  *>(&client_infot);
    client_event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cfd, &client_event);
}

