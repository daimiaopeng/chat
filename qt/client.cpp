#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent, Socket *socket) :
        QDialog(parent),
        ui(new Ui::Client),
        _socket(socket)
{
        ui->setupUi(this);
        QObject::connect(socket, SIGNAL(code4(json)), this, SLOT(code4(json)));
        QObject::connect(this, SIGNAL(getRegisterNums()), _socket, SLOT(getRegisterNums()));
}

Client::~Client() {
    delete ui;
}

void Client::code4(json _json){
    int nums =_json["nums"];
    qDebug()<<nums;
    ui->label_6->setText(QString(nums));
}



void Client::on_pushButton_clicked()
{

    emit getRegisterNums();

}
