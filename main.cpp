#include <iostream>
#include <sys/socket.h>
#include "Server.h"
#include <thread>

using namespace std;
int main() {

    Server server("0.0.0.0",6668);
    server.run();



    return 0;
}