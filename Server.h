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
#define BUFF_MAX 1024
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


class Server {
private:
    char *_ip;
    int lfd;
    u_int _port;
    struct sockaddr_in serv_addr{};
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
    }


    void run();
    void eventset();
    void eventadd();
    void eventdel();

    void eventadd(int lfd);

    void eventadd(int lfd, int epoll_fd);
};


#endif //CHAT_SERVER_H
