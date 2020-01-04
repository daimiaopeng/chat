#include "mainwindow.h"
#include "client.h"
#include <QApplication>
#include <QCoreApplication>
#include "socket.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Socket socket;
    Client c(nullptr,&socket);
    MainWindow w(nullptr, &socket,&c);
    w.show();

    return app.exec();
}
