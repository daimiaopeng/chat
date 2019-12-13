//
// Created by daimiaopeng on 2019/11/4.
//

#include "MessageQueue.h"
#include "Server.h"

void MessageQueue::push(const string &str) {
    lock_guard<mutex> lock(_mutex);
//    MessageJson messageJson(str);
//    cout<<messageJson.jsontostr.dump()<<endl;
//    std::lock_guard<std::mutex> guard(g_pages_mutex);
    redis.pushMessageQueue(str);
    LOG(INFO) << "添加成功 :" << str;
}

void MessageQueue::run() {
    thread _thread(&MessageQueue::sendstr, this);
    _thread.detach();
}

void MessageQueue::sendstr() {
    for (;;) {
        this_thread::sleep_for(chrono::milliseconds(100));
        lock_guard<mutex> lock(_mutex);
        int messageLen = redis.lenMessage();
        if (messageLen != 0) {
            string str = redis.popMessageQueue();
            LOG(INFO) << "从队列取出：" << str;
            sendMessage(str);
        }
    }
}

void MessageQueue::sendMessage(const string &str) {
    //bug 例如adswer 没有分隔符程序异常退出
    int index = str.find_first_of(" ");
    string name = str.substr(0, index);
    string message = str.substr(index + 1, str.length() - 1);
    if (name == "all") {
        for (int i = 0; i < len; i++) {
            if (g_events[i].status == 0) continue;
            send(g_events[i].fd, message.c_str(), message.size(), 0);
        }
        LOG(INFO) << "发送给全部：" << message;
        return;
    } else {
        int fd = redis.getFd(name);
        for (int i = 0; i < len; i++) {
            if (fd == g_events[i].fd && fd != -1) {
                if (g_events[i].status == 0) break;
                send(fd, message.c_str(), message.size(), 0);
                LOG(INFO) << "发送给:" << name << "发送消息：" << message;
            }
        }
    }
}
