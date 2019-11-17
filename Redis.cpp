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

void Redis::pushMessageQueue(string message) {
    string str = "RPUSH messageQueue " + message;
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, str.c_str()));
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
