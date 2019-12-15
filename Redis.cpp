//
// Created by daimiaopeng on 2019/11/17.
//

#include "Redis.h"

void Redis::setJson(json json) {
//    string s_id = json["s_id"];
//    string r_id = json["r_id"];
//    string data = json["data"];
//
//    string str = "hset "+id+" "+json.dump();

}

void Redis::pushMessageQueue(const string &message) {
    // 注意转义
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, "RPUSH messageQueue %s", message.c_str()));
    LOG(WARNING) << "添加成功 :" << message;
    freeReplyObject(reply);
}

int Redis::lenMessage() {
    string str = "LLEN messageQueue";
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, str.c_str()));
    int len = reply->integer;
    freeReplyObject(reply);
    return len;
}

string Redis::popMessageQueue() {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, "LPOP  messageQueue"));
    string str = reply->str;
    LOG(WARNING) << "从队列取出：" << str;
    freeReplyObject(reply);
    return str;
}

string Redis::getName(int fd) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("get " + to_string(fd)).c_str()));
    string str;
    if (reply->integer == 0) {
        str = "";
    }
    str = reply->str;
    freeReplyObject(reply);
    return str;
}

void Redis::setName(int fd, const string &name) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn,
                                                               string("set " + name + " " + to_string(fd)).c_str()));
    //name传进来"root\n" 预期"root"
//    if (reply->integer !=0){
//        LOG(INFO) << "设置名字：" << name<<" fd:"<<fd;
//
//    }else{
//        LOG(INFO) << "失败 设置名字：" << name<<" fd:"<<fd;
//    }
    freeReplyObject(reply);
}


void Redis::delName(int fd) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("del " + to_string(fd)).c_str()));
    freeReplyObject(reply);
}

int Redis::getFd(const string &name) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("get " + name).c_str()));
    string str;
    if (reply->str == nullptr) {
        str = "-1";
    } else {
        str = reply->str;
    }
    freeReplyObject(reply);
    return stoi(str);
}

string Redis::login(const string &name, const string &passwd) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("get " + name).c_str()));
    if (reply->str == nullptr) {
        LOG(INFO) << name + " 该用户没有注册";
        return "";
    }
    string str = reply->str;
    if (passwd == str) {
        LOG(INFO) << name + " 登录成功";
        string token = "test123ihsdfasdf";
        setToken(token, name);
        return token;
    } else {
        LOG(INFO) << name + " 登录密码错误";
        return "";
    }
}

int Redis::registered(const string &name, const string &passwd) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("get " + name).c_str()));
    if (reply->str != nullptr) {
        LOG(INFO) << name + " 该用户已注册";
        return 0;
    }
    reply = static_cast<redisReply *>(redisCommand(conn, string("set " + name + " " + passwd).c_str()));
    LOG(INFO) << name + " 注册成功";
    freeReplyObject(reply);
    return 1;
}

string Redis::getName(const string &token) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("get " + token).c_str()));
    string str;
    if (reply->integer == 0) {
        str = "";
    }
    str = reply->str;
    LOG(INFO) << token + "(token->name)" << str;
    freeReplyObject(reply);
    return str;
}

void Redis::setToken(const string &token, const string &name) {
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, string("set " + token + " " + name).c_str()));
    LOG(INFO) << name + " (name->token)" << token;
    //name传进来"root\n" 预期"root"
//    if (reply->integer !=0){
//        LOG(INFO) << "设置名字：" << name<<" fd:"<<fd;
//
//    }else{
//        LOG(INFO) << "失败 设置名字：" << name<<" fd:"<<fd;
//    }
    freeReplyObject(reply);
}
