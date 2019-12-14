//
// Created by daimiaopeng on 2019/12/12.
//
//{"type":1}
#include "MessageJson.h"

//json MessageJson::tostr() {
//    return toJson;
//}


string MessageJson::code0() {
    string name = _json["data"]["name"];
    string passwd = _json["data"]["passwd"];
    int code = redis.login(name, passwd);
    json reJsonStr;
    if (code == 1) {
        reJsonStr = {{"code", code},
                     {"data", {{"message", "登录成功"}}}};
        return reJsonStr.dump();
    } else {
        reJsonStr = {{"code", code},
                     {"data", {{"message", "登录失败"}}}};
    }
    return reJsonStr.dump();
}

string MessageJson::res() {
    code = _json["code"];
    switch (code) {
        case 0:
            type = "login";
            return code0();
        case 1:
            return "1";
    }
    return "string MessageJson::res()";
}
