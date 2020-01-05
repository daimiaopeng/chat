//
// Created by daimiaopeng on 2019/12/12.
//
#include "MessageJson.h"

MessageJson::MessageJson(const Redis &redis, const string &str) : redis(redis) {
    try {
        _json = json::parse(str);
        _isParseSuccess = true;
    }
        //test
    catch (std::exception &e) {
        _isParseSuccess = false;
        LOG(ERROR) << "消息格式错误: " << e.what();
    }
}

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
                type = "getOnile";
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
}

string MessageJson::code0() {
    //登录接口
//    {"code":0,"data":{"passwd":"123456","name":"daimiaopeng"}}
//    {"code":0,"data":{"passwd":"qwe","name":"qwe"}}
    string name = _json["data"]["name"];
    string passwd = _json["data"]["passwd"];
    string message;
    json reJsonStr;

    string token = redis.login(name, passwd);
    if (!token.empty()) {
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
    //注册接口
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
    //获取在线人数和列表
    //{"code":2,"token":"qwe"}
//   return {"code":2,"data":["qwe"],"nums":1}
    auto all = redis.geOnile();
    json reJsonStr;
    reJsonStr["code"] = code;
    int nums=0;
    json j_array{};
    for(auto i:all){
        nums++;
        j_array.push_back(i);
    }
    reJsonStr["data"] = j_array;
    reJsonStr["nums"] = nums;
    return reJsonStr.dump();
}

string MessageJson::code3() {
    // 发送消息给某个人接口
//    {"code":3,"token":"qwe","receiver":"daimiaopeng","data":"hello"}
//    {"code":3,"token":"daimiaopeng","receiver":"qwe","data":"hello"}
//    write {"code":3,"data":{"message":"qwe"},"sender":"qwe"}
    json reJsonStr;
    reJsonStr["code"] = code;
    reJsonStr["data"]["message"] = _json["data"];
    reJsonStr["sender"] = _json["sender"];
    return reJsonStr.dump();
}

string MessageJson::code4() {
    //获取注册人数接口
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

    auto receiver = _json.find("receiver");
    if (receiver == _json.end()) {
        _json["receiver"] = _json["sender"];
    }

    LOG(INFO) << "set sender name=" << name << " ip=" << infor->ip << " port=" << infor->port;
    return _json.dump();
}

bool MessageJson::isParseSuccess() {
    return _isParseSuccess;
}

json MessageJson::getJson() {
    return _json;
}






