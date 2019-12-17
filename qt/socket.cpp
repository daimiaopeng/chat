#include "socket.h"

Socket::Socket(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket();
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    tcpSocket->connectToHost("127.0.0.1", 6668, QTcpSocket::ReadWrite);

}

void Socket::connected() {
    qDebug() << "connected" << "\n";
}

void Socket::readData() {
    QString readMessage = tcpSocket->readAll();
    qDebug() << "readMessage: " << readMessage << "\n";
    try {
        _json = json::parse(readMessage.toStdString());
    } catch (std::exception &e) {
        qDebug() << e.what();
        return;
    }

    int code = _json["code"];
    switch (code) {
        case 0:
            emit code0(_json);
            qDebug() << "login";
        case 1:
            emit code1(_json);
            qDebug() << "registered";
        default:
            qDebug() << "default";
    }
}

void Socket::login(QString name, QString passwd) {
    json message;
    message["code"] = 0;
    message["data"]["name"] = name.toStdString();
    message["data"]["passwd"] = passwd.toStdString();
    QString writeMessage = QString::fromStdString(message.dump());
    qDebug() << "writeData: " << writeMessage;
    writeData(writeMessage);
}

void Socket::reg(QString name, QString passwd) {
    json message;
    message["code"] = 1;
    message["data"]["name"] = name.toStdString();
    message["data"]["passwd"] = passwd.toStdString();
    QString writeMessage = QString::fromStdString(message.dump());
    qDebug() << "writeData: " << writeMessage;
    writeData(writeMessage);
}


void Socket::writeData(QString message) {
    tcpSocket->write(message.toLatin1());
    tcpSocket->waitForBytesWritten(1);
}


