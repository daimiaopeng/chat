//
// Created by daimiaopeng on 2019/11/4.
//

#include "MessageQueue.h"

void MessageQueue::push(const string &str, event_infor *infor) {
    lock_guard<mutex> lock(_mutex);
    MessageJson messageJson(redis, str);
    if (messageJson.isParseSuccess() == true) {
        string messageNew = messageJson.messageNew(infor);
        if (messageNew=="giveUp"){
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
    if (messageJson.isParseSuccess() == false) {
        return;
    }
    string writeData = messageJson.res();
    int fd;
    string receiver = messageJson.getJson()["receiver"];
    if (receiver == "None") {
        fd = messageJson.getJson()["fd"];
    } else {
        fd = redis.getFd(messageJson.getJson()["receiver"]);
    }
    _sendPeople(fd, writeData);

//    int index = str.find_first_of(" ");
//    string name = str.substr(0, index);
//    string message = str.substr(index + 1, str.length() - 1);
//    if (name == "all") {
//        for (int i = 0; i < len; i++) {
//            if (g_events[i].status == 0) continue;
//            send(g_events[i].fd, message.c_str(), message.size(), 0);
//        }
//        LOG(INFO) << "发送给全部：" << message;
//        return;
//    } else {
//        int fd = redis.getFd(name);
//        if (fd == -1){
//            //在这里用push会死锁
////            redis.pushMessageQueue(str);
//            return;
//        }
//        for (int i = 0; i < len; i++) {
//            if (fd == g_events[i].fd && fd != -1) {
//                if (g_events[i].status == 0) break;
//                send(fd, message.c_str(), message.size(), 0);
//                LOG(INFO) << "发送给:" << name << "发送消息：" << message;
//            }
//        }
//    }
}

void MessageQueue::_sendPeople(int fd, const string &writeData) {
    for (int i = 0; i < len; i++) {
        if (g_events[i].status == 0 || g_events[i].fd != fd) continue;
        LOG(INFO) << "发送fd：" << g_events[i].fd<<"数据:"<<writeData;
        send(g_events[i].fd, writeData.c_str(), writeData.size(), 0);
    }
}