//
// Created by daimiaopeng on 2019/12/12.
//
//{"type":1}
#include "MessageJson.h"

//json MessageJson::tostr() {
//    return toJson;
//}
void MessageJson::_set() {
    sender = jsontostr["data"]["sender"];
    receiver = jsontostr["data"]["receiver"];
}
