#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "json.hpp"
#include <thread>

using boost::asio::ip::tcp;

enum {
    max_length = 1024
};
using namespace std;

int main() {
    boost::asio::io_context io_context;
    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("127.0.0.1", "6666"));

    thread t([&]() { io_context.run(); });
    t.detach();
    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_length = std::strlen(request);
//    boost::asio::write(s, boost::asio::buffer(request, request_length));
    for (;;) {
        char reply[max_length]{};

        size_t reply_length = s.read_some(boost::asio::buffer(reply, request_length));
        if (reply_length == 1) {
            continue;
        }
        std::cout << "len = " << reply_length;
        std::cout << " Reply is: ";
        std::cout << reply;
//        std::cout.write(reply, reply_length);
        std::cout << "\n";
    }


    return 0;
}