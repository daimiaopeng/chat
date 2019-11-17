#include <iostream>
#include <sys/socket.h>
#include "Server.h"
#include <thread>
#include "Redis.h"

using namespace std;


int main() {
    Redis redis("127.0.0.1", 6379);
    MessageQueue messageQueue(redis);
    Server server("0.0.0.0", 6666, &messageQueue);


    server.run();

    return 0;
}