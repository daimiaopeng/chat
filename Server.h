//
// Created by daimiaopeng on 2019/10/31.
//


#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "MessageQueue.h"

#include "event_infor.h"
#include "glog/logging.h"
#include <vector>
#include<algorithm>

class Server {
private:
    string _ip;
    int lfd;
    u_int _port;
    struct sockaddr_in serv_addr{};
    int epoll_fd;
    struct event_infor g_events[MAX_EVENTS + 1]{};
    MessageQueue *_messageQueue;
public:
    Server(string ip, u_int port, const MessageQueue *messageQueue);

    void run();

    void eventset();

    void eventdel();

    void initsocket();

    void recvdata(event_infor *infor);

    void acceptconn(event_infor *infor);

    void eventadd(int events, event_infor *infor);

    void senddata(event_infor *infor);
};


#endif //CHAT_SERVER_H
