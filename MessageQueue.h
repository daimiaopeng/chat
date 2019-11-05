//
// Created by daimiaopeng on 2019/11/4.
//

#ifndef CHAT_MESSAGEQUEUE_H
#define CHAT_MESSAGEQUEUE_H

#include <deque>
#include <queue>
#include <string>
#include <thread>
#include <mutex>

using namespace std;


struct event_infor;


class MessageQueue {
private:
    queue<string> _queue;
    event_infor *g_events;
    int len;

    mutex _mutex;
public:
    MessageQueue() = default;

    void init(event_infor *g_events, int len) {
        this->g_events = g_events;
        this->len = len;
    };

    void push(char str[]);

    void run();

    void sendstr();
};


#endif //CHAT_MESSAGEQUEUE_H
