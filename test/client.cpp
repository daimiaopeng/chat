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

int main(int argc,char *argv[])
{
    int sockfd,numbytes;
    char buf[BUFSIZ];
    struct sockaddr_in their_addr;
    printf("break!");
    while((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1);
    printf("We get the sockfd~\n");
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(6668);
    their_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bzero(&(their_addr.sin_zero), 8);

    while(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1);
    printf("Get the Server~Cheers!\n");
    int i  =0;
    while(1)
    {
        char a[1000] = "{\"code\":0,\"data\":{\"passwd\":\"123456\",\"name\":\"daimiaopeng\"}}\0";
        numbytes = send(sockfd, a, strlen(a), 0);
        numbytes=recv(sockfd,buf,BUFSIZ,0);
        buf[numbytes]='\0';
        printf("%d received:%s\n",i,buf);
        i++;
//        numbytes = recv(sockfd, buf, BUFSIZ,0);//接收服务器端信息
//        buf[numbytes]='\0';
//        printf("%s",buf);
    }

    return 0;
}