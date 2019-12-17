#include "mainwindow.h"
#include "client.h"
#include <QApplication>
#include <QCoreApplication>
#include "socket.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Socket socket;
    MainWindow w(nullptr, &socket);

//    Client client;
//    client.show();
//    qApp-> quit();
    w.show();
    return app.exec();
}
