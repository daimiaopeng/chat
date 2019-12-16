//
// Created by daimiaopeng on 2019/12/16.
//

#ifndef CHAT_PROJECT_EVENT_INFOR_H
#define CHAT_PROJECT_EVENT_INFOR_H

#include <cctype>
#include <string>
#include <future>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <cstdio>
#include <memory>

#define BUFF_MAX 1024
#define MAX_EVENTS 1280
struct event_infor;
typedef std::function<void(event_infor *)> EventCallback;
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
    std::string id;
};
#endif //CHAT_PROJECT_EVENT_INFOR_H
