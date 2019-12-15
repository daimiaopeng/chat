//
// Created by daimiaopeng on 2019/12/12.
//
#include "MessageJson.h"

string MessageJson::res() {
    code = _json["code"];
    switch (code) {
        case 0:
            type = "login";
            return code0();
        case 1:
            type = "registered";
            return code1();
    }
    return "string MessageJson::res()";
}

string MessageJson::code0() {
//    {"code":0,"data":{"passwd":"123456","name":"daimiaopeng"}}
    string name = _json["data"]["name"];
    string passwd = _json["data"]["passwd"];
    string token = redis.login(name, passwd);
    string message;
    json reJsonStr;
    if (token != "") {
        message = "登录成功";
    } else {
        message = "登录失败";
    }
    reJsonStr = {{"code",  code},
                 {"token", token},
                 {"data",  {{"message", message}}}};
    return reJsonStr.dump();
}

string MessageJson::code1() {
//    {"code":1,"data":{"passwd":"123412356","name":"qwieuy"}}
    string name = _json["data"]["name"];
    string passwd = _json["data"]["passwd"];
    int res = redis.registered(name, passwd);
    string message;
    json reJsonStr;
    if (res == 1) {
        message = "注册成功";
    } else {
        message = "注册失败";
    }
    reJsonStr = {{"code", code},
                 {"data", {{"message", message}}}};

    return reJsonStr.dump();
}


