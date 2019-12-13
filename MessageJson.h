//
// Created by daimiaopeng on 2019/12/12.
//

#ifndef CHAT_PROJECT_MESSAGEJSON_H
#define CHAT_PROJECT_MESSAGEJSON_H

#include "json.hpp"
#include <string>
#include <iostream>

using json = nlohmann::json;
using namespace std;

class MessageJson {
public:
    int type;
    string sender;
    string receiver;
    json toJson;
    json jsontostr;
public:
//    MessageJson(int type, string send, string receive) : type(type), send(sender), receive(receive) {
//        toJson = {{"type", type},{"data", {{"send", send},{"receive", receive}}}};
//    }
    MessageJson(string str) {
        jsontostr = json::parse(str);
        type = jsontostr["type"];
        if (type == 1) {
            _set();
        }
    }

protected:
    void _set();
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


};


#endif //CHAT_PROJECT_MESSAGEJSON_H
