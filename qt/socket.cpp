#include "socket.h"
#include<cstring>
Socket::Socket(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket();
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

}
void Socket::init(QString ip,int port){
    tcpSocket->connectToHost(ip, port, QTcpSocket::ReadWrite);
}

void Socket::connected() {
    qDebug() << "connected" << "\n";
}

void Socket::readData() {
    qDebug() << "readMessage";
    MessageStruct messageStruct;
    tcpSocket->read((char*)&messageStruct,sizeof(MessageStruct));
    char *jsonData = static_cast<char *>(malloc(messageStruct.jsonLen));
    tcpSocket->read(jsonData,messageStruct.jsonLen);
    string readMessage(jsonData,messageStruct.jsonLen);
    free(jsonData);
    qDebug() << "readMessage: " << QString::fromStdString(readMessage)<< "\n";
    try {
        _json = json::parse(readMessage);
    } catch (std::exception &e) {
        qDebug() << e.what();
        return;
    }
    int code = _json["code"];
    switch (code) {
        case 0:
            emit code0(_json);
            qDebug() << "login";
            break;
        case 1:
            emit code1(_json);
            qDebug() << "registered";
            break;
        case 2:
            emit code2(_json);
            qDebug() << "registered";
            break;
        case 3:
            emit code3(_json);
            break;
        case 4:
            emit code4(_json);
            qDebug() << "getRegisterNums";
            break;
        case 5:
            emit code5(_json);
            qDebug() << "getFile";
            break;
        default:
            qDebug() << "default";
            break;
    }
}
void Socket::getOnile(){
    json message;
    message["code"] = 2;
    message["token"] = token.toStdString();
    QString writeMessage = QString::fromStdString(message.dump());
    qDebug() << "writeData: " << writeMessage;
    writeData(writeMessage);
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


void Socket::getRegisterNums(){
    json message;
    message["code"] = 4;
    message["token"] = token.toStdString();
    QString writeMessage = QString::fromStdString(message.dump());
    qDebug() << "getRegisterNums: " << writeMessage;
    writeData(writeMessage);
}

void Socket::writeData(QString message) {
    string data = message.toStdString();
    int len = sizeof(MessageStruct) + data.size();
    MessageStruct *messageStruct = static_cast<struct MessageStruct *>(malloc(len));
    messageStruct->jsonLen = data.size();
    strncpy(messageStruct->json, data.c_str(), data.size());
    tcpSocket->write((char*) messageStruct,len);
    tcpSocket->flush();
    delete [] messageStruct;
}


