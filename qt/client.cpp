#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Client) {

    ui->setupUi(this);

}

Client::~Client() {
    delete ui;
}

void Client::pustText() {
    str = tcpSocket->readAll();
    ui->textEdit_2->setText(str);
    qDebug() << "str: " << str << "\n";
}

void Client::on_pushButton_clicked() {
    QString input_data = ui->textEdit->toPlainText();

    QString receiver = ui->lineEdit->text();
    QString name = ui->lineEdit_2->text();
//  qDebug()<<receiver<<"\n";
    if (receiver != "") {
        send(receiver + " " + input_data);
        tcpSocket->waitForBytesWritten(1);
    }

    if (name != "") {
        send("setname " + name + "\n");
        tcpSocket->waitForBytesWritten(1);
    }

}

void Client::send(QString str) {
    qDebug() << "send str: " << str << "";
    tcpSocket->write(str.toLatin1());

}

QString Client::read() {
    QString str = tcpSocket->readAll();
    return str;
}

void Client::connected() {
    qDebug() << "connected()";
}
