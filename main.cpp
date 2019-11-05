#include <iostream>
#include <sys/socket.h>
#include "Server.h"
#include <thread>

using namespace std;


int main() {

    Server server("0.0.0.0", 6666);

    server.run();
//    auto a = std::bind(&test);
//    a();

    return 0;
}