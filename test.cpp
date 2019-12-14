//
// Created by daimiaopeng on 2019/12/12.
//

#ifndef CHAT_TEST
#define CHAT_TEST
#define _GLIBCXX_USE_CXX11_ABI 1

#include <iostream>
#include "Redis.h"
#include "MessageJson.h"

using namespace std;

int main() {
//
    Redis redis("127.0.0.1", 6379);
    string a;
    string b;
    cin >> a;
//    {"code":0,"data":{"passwd":"123","name":"daimiaopeng"}}
    MessageJson messageJson(redis, a);
    string writeData = messageJson.res();
    cout << writeData;

    return 0;
}

#endif