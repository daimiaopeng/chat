//
// Created by daimiaopeng on 2019/10/31.
//
#include <unistd.h>
#include <cctype>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <sys/epoll.h>
#include <stdio.h>
#include <memory>

#define BUFF_MAX 1024
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

struct event_infor;

typedef std::function<void(event_infor *)> EventCallback;

struct event_infor {
    std::string ip;
    u_int port;
    int fd;
    EventCallback eventCallback;
    int events;
//    void *infor;
};


class Server {
private:
    char *_ip;
    int lfd;
    u_int _port;
    struct sockaddr_in serv_addr{};
    int epoll_fd;

public:
    Server(char *ip, u_int port) : _ip(ip), _port(port) {
        lfd = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_addr.s_addr = inet_addr(_ip);
        serv_addr.sin_port = htons(_port);
        serv_addr.sin_family = AF_INET;
        printf("Using ip:%s port: %d\n", _ip, _port);
        bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        listen(lfd, 128);
        epoll_fd = epoll_create(128);
    }


    void run();
    void eventset();
    void eventdel();


    void initsocket();
    void recvdata(event_infor *infor);

    void test();

    void acceptconn(event_infor *infor);

    void initsocket(event_infor &l_infor);

    void eventadd(int events, event_infor &infor);

    void eventadd(int events, event_infor *infor);

    void initsocket(event_infor *l_infor);
};


#endif //CHAT_SERVER_H
