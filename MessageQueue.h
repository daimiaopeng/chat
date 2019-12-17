//
// Created by daimiaopeng on 2019/11/4.
//

#ifndef CHAT_MESSAGEQUEUE_H
#define CHAT_MESSAGEQUEUE_H

#include "hiredis/hiredis.h"
#include <deque>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include "Redis.h"
#include "MessageJson.h"
#include "event_infor.h"

using namespace std;

class Server;

struct event_infor;

class MessageQueue {
private:
    queue<string> _queue;
    event_infor *g_events;
    int len;
    mutex _mutex;

public:
    Redis redis;
public:
    MessageQueue(Redis redis) : redis(redis) {};

    void init(event_infor *g_events, int len) {
        this->g_events = g_events;
        this->len = len;
    };

    void push(const string &str, event_infor *infor);

    void run();

    void sendstr();


    void sendMessage(const string &str);

protected:
    void _sendPeople(int fd, const string &writeData);

    void send1(int fd, string &str);
};


#endif //CHAT_MESSAGEQUEUE_H
