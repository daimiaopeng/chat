//
// Created by daimiaopeng on 2019/12/12.
//
#include "MessageJson.h"

string MessageJson::res() {
    try {
        code = _json["code"];
        switch (code) {
            case 0:
                type = "login";
                return code0();
            case 1:
                type = "registered";
                return code1();
            case 2:
                type = "sendALl";
                return code2();
            case 3:
                type = "sendPeople";
                return code3();
            case 4:
                type = "getRegisterNums";
                return code4();
            default:
                return "";
        }
    }
    catch (std::exception &e) {
        return "错误";
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
    int isCode;
    if (res == 1) {
        isCode = 1;
        message = "注册成功";
    } else {
        isCode = 0;
        message = "注册失败,该用户名被已注册";
    }
    reJsonStr = {{"code", code},
                 {"data", {{"message", message}}}};
    reJsonStr["isCode"] = isCode;
    return reJsonStr.dump();
}

string MessageJson::code2() {
    return "接口正在开发中";
}

string MessageJson::code3() {
    return std::__cxx11::string();
}

string MessageJson::code4() {
//    {"code":4,"token":"daimiaopeng"}
    int nums = redis.getRegisterNums();
    string token = _json["token"];
    string message = "None";
    json reJsonStr;
    reJsonStr["code"] = code;
    reJsonStr["data"]["message"] = message;
    reJsonStr["nums"] = nums;
    return reJsonStr.dump();
}



string MessageJson::messageNew(event_infor *infor) {
    //认证后为消息设置其他信息
//    {"code":0,"token":"qwe","data":{"passwd":"qwe","name":"qwe"}}
//    {"code":2,"token":"qwe","data":{"passwd":"qwe","name":"qwe"}}
//    {"code":0,"data":{"passwd":"qwe","name":"qwe"}}
//    {"code":1,"data":{"passwd":"qwe","name":"qwqe"}}
//    {"code":1,"data":{"passwd":"qwse","name":"qwqe"}}
//    {"code":4,"token":"daimiaopeng"}
//    {"code":4,"token":"daimiaopeng1"}
    auto token = _json.find("token");
    string name;
    int code = _json["code"];
    if (token != _json.end()) {
        string t = *token;
        name = redis.getName(t);
        LOG(INFO) <<name;
        if (name == "") {
            //认证失败
            return "giveUp";
        } else {
            //认证成功
            redis.setName(infor->fd, name);
            _json["sender"] = name;
        }
    }else{
        if (code == 0 || code == 1) {
            _json["token"] = "None";
            _json["sender"] = "None";
        }else{
            return "giveUp";
        }
    }
    _json["ip"] = infor->ip;
    _json["fd"] = infor->fd;
    _json["port"] = infor->port;
    _json["status"] = infor->status;
    //暂时receiver = sender
    _json["receiver"] = _json["sender"];
    LOG(INFO) << "set sender name=" << name << " ip=" << infor->ip << " port=" << infor->port;
    return _json.dump();
}

bool MessageJson::isParseSuccess() {
    return _isParseSuccess;
}

json MessageJson::getJson() {
    return _json;
}




