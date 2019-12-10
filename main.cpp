#include <iostream>
#include <sys/socket.h>
#include "Server.h"
#include <thread>
#include "Redis.h"
#include "glog/logging.h"

using namespace std;

void initLog() {
    google::InitGoogleLogging("chatLog");    //初始化log的名字为daqing
    google::SetLogDestination(google::GLOG_INFO, "../log");    //设置输出日志的文件夹，文件夹必须已经存在
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogFilenameExtension("log_");
    FLAGS_colorlogtostderr = true;  // Set log color
    FLAGS_logbufsecs = 0;  // Set log output speed(s)
    FLAGS_max_log_size = 1024;  // Set max log file size
    FLAGS_stop_logging_if_full_disk = true;  // If disk is ful
}

int main() {
    initLog();
    Redis redis("127.0.0.1", 6379);
//    Redis redis("120.77.204.248", 6379,"passwd");
    MessageQueue messageQueue(redis);
    Server server("0.0.0.0", 6666, &messageQueue);

    server.run();

    return 0;
}