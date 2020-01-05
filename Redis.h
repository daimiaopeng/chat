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

class Redis {
private:
    string ip;
    u_int port;
    redisContext *conn;

public:
    Redis(string ip, u_int port, string passwd);

    Redis(string ip, u_int port);

    void init();

    void pushMessageQueue(const string &message);

    string getName(int fd);

    string getName(const string &token);

    int getFd(const string &name);

    void setName(int fd, const string &name);

    void delName(int fd);

    set<string> geOnile();

    redisContext *getConn() { return conn; };

    auto redisReply_ptr(void *reply);

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
