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
    cout << reply->integer;
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
    string str = reply->str;
//    if (reply->integer == 0) {
//        str = "-1";
//    } else{
//        str = reply->str;
//    }
    freeReplyObject(reply);
    return stoi(str);
}
