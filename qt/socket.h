#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include<QtNetwork/QTcpSocket>
#include"json.hpp"
#include <string>

using json = nlohmann::json;
using namespace std;
struct MessageStruct{
    int jsonLen;
    char json[0];
};
class Socket : public QObject {
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    QTcpSocket *tcpSocket;
    void writeData(QString message);
    void init(QString ip,int port);
    QString receiver;
    QString userName;
    QString token;
signals:
    void code0(json _json);
    void code1(json _json);
    void code2(json _json);
    void code3(json _json);
    void code4(json _json);
    void code5(json _json);
public
    slots:
    void readData();
    void connected();
    void login(QString name, QString passwd);
    void reg(QString name, QString passwd);
    void getRegisterNums();
    void getOnile();
protected:
    json _json;



};

#endif // SOCKET_H
