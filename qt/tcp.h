#ifndef TCP_H
#define TCP_H

#include<QtNetwork/QTcpSocket>
#include <QDialog>
#include <QMainWindow>


class Tcp : public QDialog {

protected:

    QTcpSocket *tcpSocket;
    QString message;

private
    slots:

            void

    newConnect(); //连接服务器
    void readMessage();  //接收数据


public:
    Tcp();

    void send(QString str);

    QString read();

    ~Tcp();
};

#endif // TCP_H
