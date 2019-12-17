#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include<QtNetwork/QTcpSocket>

namespace Ui {
    class Client;
}

class Client : public QDialog {
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

    ~Client();

private
    slots:

//    void newConnect(); //连接服务器
//    void readMessage();  //接收数据
            void

    on_pushButton_clicked();

    void connected();

    void pustText();

private:
    QTcpSocket *tcpSocket;
    Ui::Client *ui;
    QString str;

    void displayError(QAbstractSocket::SocketError);  //显示错误
    void send(QString str);

    QString read();

};

#endif // CLIENT_H
