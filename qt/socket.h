#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include<QtNetwork/QTcpSocket>
#include"json.hpp"
#include <string>

using json = nlohmann::json;
using namespace std;

class Socket : public QObject {
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);

    void writeData(QString message);


    signals:

            void
    code0(json
    _json);

    void code1(json _json);

public
    slots:

            void

    readData();

    void connected();

    void login(QString name, QString passwd);

    void reg(QString name, QString passwd);

protected:
    json _json;
    QTcpSocket *tcpSocket;
};

#endif // SOCKET_H
