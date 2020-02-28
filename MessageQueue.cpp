//
// Created by daimiaopeng on 2019/11/4.
//

#include "MessageQueue.h"


MessageQueue::MessageQueue(Redis &redis) : redis(redis) {}

void MessageQueue::push(const string &str, event_infor *infor) {
    lock_guard<mutex> lock(_mutex);
    MessageJson messageJson(redis, str);
    if (messageJson.isParseSuccess()) {
        string messageNew = messageJson.messageNew(infor);
        if (messageNew == "giveUp") {
            return;
        }
        redis.pushMessageQueue(messageNew);
    }
}

void MessageQueue::run() {
    thread _thread(&MessageQueue::sendstr, this);
    _thread.detach();
}

void MessageQueue::sendstr() {
    for (;;) {
        this_thread::sleep_for(chrono::milliseconds(10));
        lock_guard<mutex> lock(_mutex);
        int messageLen = redis.lenMessage();
        if (messageLen != 0) {
            string str = redis.popMessageQueue();
            sendMessage(str);
        }
    }
}

void MessageQueue::sendMessage(const string &str) {
    MessageJson messageJson(redis, str);
    if (!messageJson.isParseSuccess()) {
        return;
    }
    string writeData = messageJson.res();
    int fd;
    json tempJson = messageJson.getJson();
    string receiver = tempJson["receiver"];
    if (receiver == "None") {
        fd = tempJson["fd"];
    } else {
        fd = redis.getFd(tempJson["receiver"]);
    }
    _sendPeople(fd, writeData);
}

void MessageQueue::_sendPeople(int fd, const string &writeData) {
    for (int i = 0; i < _len; i++) {
        if (_g_events[i].status == 0 || _g_events[i].fd != fd) continue;
        LOG(INFO) << "发送fd：" << _g_events[i].fd << "数据:" << writeData;
//        send(_g_events[i].fd, writeData.c_str(), writeData.size(), 0);
        finalSend(_g_events[i].fd, writeData.c_str(), writeData.size(), 0);
    }
}

void MessageQueue::init(event_infor *g_events, int len) {
    this->_g_events = g_events;
    this->_len = len;
}

void MessageQueue::finalSend(int fd,string message,int size,int flages){
    int len = sizeof(MessageStruct) + message.size();
    MessageStruct *messageStruct = static_cast<struct MessageStruct *>(malloc(len));
    messageStruct->jsonLen = message.size();
    strncpy(messageStruct->json, message.c_str(), message.size());
    send(fd, messageStruct, len, flages);
    free(messageStruct) ;
}
