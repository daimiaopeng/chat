//
// Created by daimiaopeng on 2019/11/17.
//

#ifndef CHAT_PROJECT_REDIS_H
#define CHAT_PROJECT_REDIS_H

#include <string>
#include "json.hpp"
#include <hiredis/hiredis.h>
#include <string>
#include "glog/logging.h"

using json = nlohmann::json;
using namespace std;

class Redis {
private:
    string ip;
    u_int port;
    redisContext *conn;
public:
    Redis(string ip, u_int port, string passwd) : ip(ip), port(port) {
        conn = redisConnect(ip.c_str(), port);
        if (conn->err) printf("connection error:%s\n", conn->errstr);
        redisReply *reply = static_cast<redisReply *>(redisCommand(conn, "AUTH %s", passwd.c_str()));
        if (reply->type == REDIS_REPLY_ERROR) {
            LOG(INFO)<<"Redis认证失败！";
        } else {
            LOG(INFO)<<"Redis认证成功！";
        }
        freeReplyObject(reply);
    };

    Redis(string ip, u_int port) : ip(ip), port(port) {
        conn = redisConnect(ip.c_str(), port);
        if (conn->err) LOG(ERROR) <<"connection error: "<<conn->errstr;
    };

    void setJson(json json_str);

    void pushMessageQueue(string message);

    redisContext *getConn() { return conn; };

    int lenMessage();

    string popMessageQueue();
};


#endif //CHAT_PROJECT_REDIS_H
