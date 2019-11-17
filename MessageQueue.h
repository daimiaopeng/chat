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

using namespace std;


struct event_infor;

class MessageQueue {
private:
    queue<string> _queue;
    event_infor *g_events;
    int len;
    Redis redis;
    mutex _mutex;
public:
    MessageQueue(Redis redis) : redis(redis) {};

    void init(event_infor *g_events, int len) {
        this->g_events = g_events;
        this->len = len;
    };

    void push(char str[]);

    void run();

    void sendstr();
};


#endif //CHAT_MESSAGEQUEUE_H
