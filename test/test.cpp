//
// Created by daimiaopeng on 2019/12/12.
//

#ifndef CHAT_TEST
#define CHAT_TEST
#define _GLIBCXX_USE_CXX11_ABI 1

#include <iostream>
#include "../Redis.h"
#include "../MessageJson.h"

using namespace std;

void test0(Redis &redis) {
    string name = "1223";
    string passwd = name;
    redis.login(name, passwd);
    redis.registered(name, passwd);
    string token = redis.login(name, passwd);
    redis.getName(token);
}


int main() {
//
    Redis redis("127.0.0.1", 6379);
//    string a;
//    string b;
//    cin >> a;
//    {"code":0,"data":{"passwd":"123","name":"daimiaopeng"}}
//    MessageJson messageJson(redis, a);
//    string writeData = messageJson.res();
//    cout << writeData;
    test0(redis);

    return 0;
}

#endif