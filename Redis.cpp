//
// Created by daimiaopeng on 2019/11/17.
//

#include "Redis.h"

Redis::Redis(string ip, u_int port, string passwd) : ip(ip), port(port) {
    conn = redisConnect(ip.c_str(), port);
    if (conn->err) printf("connection error:%s\n", conn->errstr);
    redisReply *reply = static_cast<redisReply *>(redisCommand(conn, "AUTH %s", passwd.c_str()));
    if (reply->type == REDIS_REPLY_ERROR) {
        LOG(INFO) << "Redis认证失败！";
    } else {
        LOG(INFO) << "Redis认证成功！";
    }
    clearToken();
    freeReplyObject(reply);
};

Redis::Redis(string ip, u_int port) : ip(ip), port(port) {
    conn = redisConnect(ip.c_str(), port);
    if (conn->err) LOG(ERROR) << "connection error: " << conn->errstr;
    clearToken();
};


auto Redis::redisReply_ptr(void *reply) {
    auto delRedisReply = [](redisReply *reply) {
        freeReplyObject(reply);
    };
    unique_ptr<redisReply, decltype(delRedisReply)> reply_unique_ptr(reinterpret_cast<redisReply *>(reply),
                                                                     delRedisReply);
    return reply_unique_ptr;
}

void Redis::pushMessageQueue(const string &message) {
    auto reply = redisReply_ptr(redisCommand(conn, "RPUSH messageQueue %s", message.c_str()));
    LOG(WARNING) << "添加到消息队列 :" << message;
}

int Redis::lenMessage() {
    string str = "LLEN messageQueue";
    auto reply = redisReply_ptr(redisCommand(conn, str.c_str()));
    int len = reply->integer;
    return len;
}

string Redis::popMessageQueue() {
    auto reply = redisReply_ptr(redisCommand(conn, "LPOP  messageQueue"));
    string str = reply->str;
    LOG(INFO) << "从消息队列取出：" << str;
    return str;
}


string Redis::getName(const string &token) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMGET token %s", token.c_str()));
    string str;
    auto element = reply->element;
    if ((*element)->str == nullptr) {
        str = "";
        return str;
    }
    str = (*element)->str;
    LOG(INFO) << token + "(token->name)" << str;
    return str;
}


int Redis::getFd(const string &name) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMGET fdName %s", name.c_str()));
    string str;
    auto element = reply->element;
    if ((*element)->str == nullptr) {
        str = "-1";
    } else {
        str = (*element)->str;
    }
    return stoi(str);
}

void Redis::setName(int fd, const string &name) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMSET fdName %s %d", name.c_str(), fd));
}

string Redis::login(const string &name, const string &passwd) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMGET login %s", name.c_str()));
    auto element = reply->element;
    if ((*element)->str == nullptr) {
        LOG(INFO) << name + " 该用户没有注册";
        return "";
    }
    string str = (*element)->str;
    if (passwd == str) {
        LOG(INFO) << name + " 登录成功";
        string token = name;
        setToken(token, name);
        return token;
    } else {
        LOG(INFO) << name + " 登录密码错误";
        return "";
    }
}

int Redis::registered(const string &name, const string &passwd) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMGET login %s", name.c_str()));
    auto element = reply->element;
    if ((*element)->str != nullptr) {
        LOG(INFO) << name + " 该用户已注册";
        return 0;
    }
    redisCommand(conn, "HMSET login %s %s", name.c_str(), passwd.c_str());
    LOG(INFO) << name + " 注册成功";
    return 1;
}


void Redis::setToken(const string &token, const string &name) {
    auto reply = redisReply_ptr(redisCommand(conn, "HMSET token %s %s", token.c_str(), name.c_str()));
    LOG(INFO) << name + " (name->token)" << token;
}

void Redis::init() {

}



int Redis::getRegisterNums() {
    auto reply = redisReply_ptr(redisCommand(conn, "HLEN login"));
    LOG(INFO) << "当前注册人数："<<reply->integer;
    return reply->integer;
}

set<string> Redis::geOnile() {
    set<string> all;
    auto reply = redisReply_ptr(redisCommand(conn, "HGETALL token"));
    auto element  = reply->element;
    for (int i =0;i<reply->elements;i= i+2){
        //取name而不是token
        all.insert(move(string(element[i+1]->str)));
    }

//    LOG(INFO) << "当前在线人： "<<(*element)->;
    return all;
}



void Redis::clearToken() {
    redisReply_ptr(redisCommand(conn, "DEL token"));
    LOG(INFO) <<"token 清空初始化成功";
}


