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

class MessageQueue {
private:
    event_infor *_g_events;
    int _len;
    mutex _mutex;

public:
    Redis redis;

public:
    explicit MessageQueue(Redis &redis);

    MessageQueue() = delete;

    void init(event_infor *g_events, int len);

    void push(const string &str, event_infor *infor);

    void run();

    void sendstr();

    void sendMessage(const string &str);

protected:
    void _sendPeople(int fd, const string &writeData);
};


#endif //CHAT_MESSAGEQUEUE_H
