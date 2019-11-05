//
// Created by daimiaopeng on 2019/11/4.
//

#include "MessageQueue.h"
#include "Server.h"

void MessageQueue::push(char str[]) {
    lock_guard<mutex> lock(_mutex);
//    std::lock_guard<std::mutex> guard(g_pages_mutex);
    _queue.push(str);
    printf("添加成功\n");
}

void MessageQueue::run() {
    thread _thread(&MessageQueue::sendstr, this);
    _thread.detach();
}

void MessageQueue::sendstr() {
    for (;;) {
        lock_guard<mutex> lock(_mutex);
        if (!_queue.empty()) {
            char *str = (char *) _queue.back().c_str();
            int str_len = _queue.back().size();
            printf("MessageQueue :%s", str);
            for (int i = 0; i < len; i++) {
                int fd = g_events[i].fd;
                if (g_events[i].status == 0) continue;
                send(fd, str, str_len, 0);
            }
            _queue.pop();
        }
    }
}
