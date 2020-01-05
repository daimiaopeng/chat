//
// Created by daimiaopeng on 2019/10/31.
//

#include "Server.h"

using namespace std;

Server::Server(string ip, u_int port, const MessageQueue *messageQueue) : _ip(ip), _port(port), _messageQueue(
        const_cast<MessageQueue *>(messageQueue)) {
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    serv_addr.sin_port = htons(_port);
    serv_addr.sin_family = AF_INET;
    LOG(INFO) << "Using ip: " << _ip << " port: " << _port;
    bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(lfd, 128);
    epoll_fd = epoll_create(128);
    _messageQueue->init(g_events, MAX_EVENTS);
    _messageQueue->run();
}


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
            }
            if (events[i].events & EPOLLOUT) {
                infor->writeCallback(infor);
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
//        LOG(ERROR)<<("%s:accept,%s\n", __func__, strerror(errno));
        LOG(ERROR) << "连接失败";
        return;
    }
    int i = 0;
    for (i = 0; i < MAX_EVENTS; i++) {
        if (g_events[i].status == 0) break;
        if (i == MAX_EVENTS) // 超出连接数上限
        {
            LOG(INFO) << __func__ << "max connect limit " << MAX_EVENTS;
            break;
        }
    }
    event_infor *cli_event_infor = &g_events[i];
    cli_event_infor->fd = cfd;
    cli_event_infor->status = true;
    cli_event_infor->port = socket_addr.sin_port;
    cli_event_infor->ip = inet_ntoa(socket_addr.sin_addr);
    cli_event_infor->readCallback = [&](event_infor *infor) { recvdata(infor); };
    cli_event_infor->writeCallback = [&](event_infor *infor) { senddata(infor); };
    LOG(INFO) << cli_event_infor->ip << " 连接成功\n";
//    eventadd(EPOLLOUT, cli_event_infor);
    eventadd(EPOLLIN, cli_event_infor);

}

void Server::recvdata(event_infor *infor) {
//    char buff[BUFF_MAX]{};
//    read(infor->fd, infor->buff,BUFF_MAX);
    int n = recv(infor->fd, infor->buff, BUFF_MAX - 1, 0);
    if (n > 0) { //收到的数据
        infor->buff[n] = '\0';
        LOG(INFO) << infor->ip << " 发来一条消息: " << infor->buff;
        _messageQueue->push(infor->buff, infor);
    } else if (n == 0) {
        LOG(INFO) << "fd: " << infor->fd << " 连接关闭";
        close(infor->fd);
        infor->status = false;
    } else {
        close(infor->fd);
        infor->status = false;
//        LOG(ERROR) << "接收数据失败";
        LOG(INFO) << "fd: " << infor->fd << " 连接关闭";
//        printf("recv[fd=%d] error[%d]:%s\n", infor->fd, errno, strerror(errno));
    }
    bzero(infor->buff, BUFF_MAX - 1);
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
//    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, infor->fd, nullptr);
//    int ep = EPOLLIN;
//    infor->events = ep;
//    eventadd(ep, infor);

}

void Server::eventadd(int events, event_infor *infor) {
    epoll_event event{0, {nullptr}};
    event.events = events;
    infor->events = events;
    event.data.ptr = static_cast<void *>(infor);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infor->fd, &event);
}


void Server::eventset() {

}

