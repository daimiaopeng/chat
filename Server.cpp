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
            event_infor *infor = static_cast<event_infor *>(events[i].data.ptr);
            if (events[i].events & EPOLLIN) {
                infor->readCallback(infor);
//                printf("readCallback\n");
            }
            if (events[i].events & EPOLLOUT) {
                infor->writeCallback(infor);
//                printf("writeCallback\n");
            }
//            auto res = async(launch::async, infor->eventCallback,infor);
            //线程不应该在这里创建，同一个fd反复创建线程
//            thread thread1(infor->eventCallback,infor);
//            thread1.detach();
            //当这个future对象被析构时，会等待线程执行完成
//            printf("123\n");
        }
    }
}


void Server::initsocket() {
    event_infor *l_infor = &g_events[MAX_EVENTS];
    l_infor->fd = lfd;
    l_infor->status = 1;
    l_infor->ip = inet_ntoa(serv_addr.sin_addr);
    l_infor->port = _port;
    l_infor->readCallback = [&](event_infor *infor) { acceptconn(infor); };
    l_infor->writeCallback = nullptr;
    eventadd(EPOLLIN, l_infor);

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
    int i = 0;
    for (i = 0; i < MAX_EVENTS; i++) {
        if (g_events[i].status == 0) break;
        if (i == MAX_EVENTS) // 超出连接数上限
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break;
        }

    }
    event_infor *cli_event_infor = &g_events[i];
    cli_event_infor->fd = cfd;
    cli_event_infor->status = 1;
    cli_event_infor->ip = inet_ntoa(socket_addr.sin_addr);
    cli_event_infor->readCallback = [&](event_infor *infor) { recvdata(infor); };
    cli_event_infor->writeCallback = [&](event_infor *infor) { senddata(infor); };
    printf("%s连接成功\n", cli_event_infor->ip.c_str());
//    eventadd(EPOLLOUT, cli_event_infor);
    eventadd(EPOLLIN, cli_event_infor);
}

void Server::recvdata(event_infor *infor) {
    char buff[BUFF_MAX]{};
//    read(infor->fd, infor->buff,BUFF_MAX);
    int n = recv(infor->fd, buff, BUFF_MAX - 1, 0);
    if (n > 0) { //收到的数据
        infor->buff[n] == '\0';
        printf("%s [%d]发来一条消息: %s\n", infor->ip.c_str(), infor->fd, buff);
        //添加到消息队列
        messageQueue.push(buff);
    } else if (n == 0) {
        close(infor->fd);
    } else {
        close(infor->fd);
        printf("recv[fd=%d] error[%d]:%s\n", infor->fd, errno, strerror(errno));
    }
//    while ((n = read(infor->fd, buff, BUFF_MAX-1)) > 1) {
//        printf("%s 发来一条消息: %s\n", infor->ip.c_str(), buff);
//        bzero(buff, sizeof(buff));
//    }
    //如果退出的话要delete

    //这里只专注与读数据
    //多线程下面这个delete有问题,recvdata会执行
    //delete(infor);
    //int res = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, infor->fd, nullptr); // 这里返回-1
//    close(infor->fd);
}

void Server::senddata(event_infor *infor) {

//    read(infor1->fd, buff,BUFF_MAX);
    if (infor->buff[0] == '\0') {
        return;
    }
    int nums = send(infor->fd, infor->buff, BUFF_MAX - 1, 0);
    bzero(infor->buff, BUFF_MAX - 1);

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, infor->fd, nullptr);
    int ep = EPOLLIN;
    infor->events = ep;
    eventadd(ep, infor);


}

void Server::eventadd(int events, event_infor *infor) {
    epoll_event event{0, {0}};
    event.events = events;
    infor->events = events;
    event.data.ptr = static_cast<void *>(infor);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infor->fd, &event);
}


void Server::eventset() {

}

