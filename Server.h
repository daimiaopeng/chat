//
// Created by daimiaopeng on 2019/10/31.
//


#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "MessageQueue.h"
#include <unistd.h>
#include <cctype>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
#include <functional>
#include <sys/epoll.h>
#include <cstdio>
#include <memory>
#include <future>
#include "glog/logging.h"
#include <vector>
#include<algorithm>

struct event_infor;

class MessageQueue;

typedef std::function<void(event_infor *)> EventCallback;
#define BUFF_MAX 1024
#define MAX_EVENTS 1280
struct event_infor {
    std::string ip;
    u_int port;
    int fd;
    bool status;
    EventCallback writeCallback;
    EventCallback readCallback;
    char buff[BUFF_MAX];
    int events;
    epoll_event *epoll_infor;
    string id;
};

class Server {
private:
    char *_ip;
    int lfd;
    u_int _port;
    struct sockaddr_in serv_addr{};
    int epoll_fd;
    struct event_infor g_events[MAX_EVENTS + 1]{};
    MessageQueue *_messageQueue;
public:
    Server(char *ip, u_int port, const MessageQueue *messageQueue) : _ip(ip), _port(port), _messageQueue(
            const_cast<MessageQueue *>(messageQueue)) {
        lfd = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_addr.s_addr = inet_addr(_ip);
        serv_addr.sin_port = htons(_port);
        serv_addr.sin_family = AF_INET;
        LOG(INFO) << "Using ip: " << _ip << " port: " << _port;
        bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        listen(lfd, 128);
        epoll_fd = epoll_create(128);
        _messageQueue->init(g_events, MAX_EVENTS);
        _messageQueue->run();
    }


    void run();

    void eventset();

    void eventdel();

    void initsocket();

    void recvdata(event_infor *infor);

    void acceptconn(event_infor *infor);

    void initsocket(event_infor &l_infor);

    void eventadd(int events, event_infor *infor);

    void initsocket(event_infor *l_infor);

    void senddata(event_infor *infor);
};


#endif //CHAT_SERVER_H
