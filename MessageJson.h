//
// Created by daimiaopeng on 2019/12/12.
//

#ifndef CHAT_PROJECT_MESSAGEJSON_H
#define CHAT_PROJECT_MESSAGEJSON_H

#include "json.hpp"
#include <string>
#include <iostream>
#include "Redis.h"

using json = nlohmann::json;
using namespace std;

class MessageJson {
public:

    int code;
    string type;
    json _json;
    Redis redis;
public:
//    MessageJson(int type, string send, string receive) : type(type), send(sender), receive(receive) {
//        toJson = {{"type", type},{"data", {{"send", send},{"receive", receive}}}};
//    }
    MessageJson(const Redis &redis, const string &str) : redis(redis) {
        _json = json::parse(str);
    }

public:
    string code0();

    string res();

protected:
//    json tostr();



//    json j2 = R"(
//      {
//            "type": type,
//            "data": {
//            "send": true,
//            "receive": 3.141
//          }
//        }
//    )"_json;
////    string a = "set foo "+j2.dump();
//    cout<<    j2["happy"]
//     <<endl;


    string code1();
};


#endif //CHAT_PROJECT_MESSAGEJSON_H
