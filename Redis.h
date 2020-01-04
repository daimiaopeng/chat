//
// Created by daimiaopeng on 2019/11/17.
//

#ifndef CHAT_PROJECT_REDIS_H
#define CHAT_PROJECT_REDIS_H

#include "json.hpp"
#include <hiredis/hiredis.h>
#include "glog/logging.h"
#include <iostream>
#include <boost/regex.hpp>
#include <memory>
#include <set>

using json = nlohmann::json;
using namespace std;

//using boost::regex;
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
            LOG(INFO) << "Redis认证失败！";
        } else {
            LOG(INFO) << "Redis认证成功！";
        }
        clearToken();
        freeReplyObject(reply);
    };

    Redis(string ip, u_int port) : ip(ip), port(port) {
        conn = redisConnect(ip.c_str(), port);
        if (conn->err) LOG(ERROR) << "connection error: " << conn->errstr;
        clearToken();
    };

    void setJson(json json_str);

    void init();

    void pushMessageQueue(const string &message);

    string getName(int fd);

    string getName(const string &token);

    int getFd(const string &name);

    void setName(int fd, const string &name);

    void delName(int fd);

    set<string> geOnile();

    redisContext *getConn() { return conn; };

    auto redisReply_ptr(void* reply);

    string login(const string &name, const string &passwd);

    int registered(const string &name, const string &passwd);

    int lenMessage();

    string popMessageQueue();

    int getRegisterNums();

    void clearToken();

    void setFd(int fd, const string &name);

    void setToken(const string &token, const string &name);


};


#endif //CHAT_PROJECT_REDIS_H
