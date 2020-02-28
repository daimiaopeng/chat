//
// Created by daimiaopeng on 2019/12/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;
struct MessageStruct {
    int jsonLen;
    char json[0];
};
int main(int argc, char *argv[]) {
    int sockfd, numbytes;
    char buf[BUFSIZ];
    struct sockaddr_in their_addr;
    printf("break!");
    while ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1);
    printf("We get the sockfd~\n");
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(6667);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(their_addr.sin_zero), 8);

    while (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1);
    printf("Get the Server~Cheers!\n");
    int i = 0;

//    string data = "{\"code\":0,\"data\":{\"passwd\":\"123456\",\"name\":\"daimiaopeng\"}}";
//    int len = sizeof(MessageStruct) + data.size();
//    MessageStruct *messageStruct = static_cast<struct MessageStruct *>(malloc(len));
//    messageStruct->jsonLen = data.size();
//    strncpy(messageStruct->json, data.c_str(), data.size());
//    cout << messageStruct->jsonLen;

    string data = "{\"code\":0,\"data\":{\"passwd\":\"123456\",\"name\":\"daimiaopeng\"}}";
    int len = sizeof(MessageStruct) + data.size();
    MessageStruct *messageStruct = static_cast<struct MessageStruct *>(malloc(len));
    messageStruct->jsonLen = data.size();
    strncpy(messageStruct->json, data.c_str(), data.size());

    for (int i = 0; i < 1; i++) {
        send(sockfd, messageStruct, len, 0);
        MessageStruct revcStruct;
        int n = recv(sockfd, &messageStruct, sizeof(revcStruct), 0);
        if (n > 0) { //收到的数据
            //获取结构体中的json长度字段
            char *jsonData = static_cast<char *>(malloc(revcStruct.jsonLen));
            //再读json数据
            recv(sockfd, jsonData, revcStruct.jsonLen, 0);
            cout << " 收到一条消息 jsonLen = " << revcStruct.jsonLen << " jsonData = " << jsonData;
            sleep(3);
            delete[](jsonData);
        }
    }
    return 0;
}