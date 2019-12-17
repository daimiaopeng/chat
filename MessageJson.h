//
// Created by daimiaopeng on 2019/12/12.
//

#ifndef CHAT_PROJECT_MESSAGEJSON_H
#define CHAT_PROJECT_MESSAGEJSON_H

#include "json.hpp"
#include "Redis.h"
#include "event_infor.h"

using json = nlohmann::json;
using namespace std;
struct event_infor;

class Redis;

class MessageQueue;

class MessageJson {
public:
    bool _isParseSuccess;
    int code;
    string type;
    json _json;
    Redis redis;
public:
//    MessageJson(int type, string send, string receive) : type(type), send(sender), receive(receive) {
//        toJson = {{"type", type},{"data", {{"send", send},{"receive", receive}}}};
//    }
    MessageJson(const Redis &redis, const string &str) : redis(redis) {
        try {
            _json = json::parse(str);
            _isParseSuccess = true;
        }
        catch (std::exception &e) {
            _isParseSuccess = false;
            LOG(ERROR) << "消息格式错误: " << e.what();
        }
    }

public:
    string messageNew(event_infor *infor);

    bool isParseSuccess();

    string res();

    json getJson();
protected:
    string code0();

    string code2();

    string code1();
};


#endif //CHAT_PROJECT_MESSAGEJSON_H
