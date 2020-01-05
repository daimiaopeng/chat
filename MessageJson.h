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
    MessageJson(const Redis &redis, const string &str);

    string messageNew(event_infor *infor);

    bool isParseSuccess();

    string res();

    json getJson();
protected:
    string code0();

    string code2();

    string code1();

    string code3();

    string code4();
};


#endif //CHAT_PROJECT_MESSAGEJSON_H
