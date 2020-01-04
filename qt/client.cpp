#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent, Socket *socket) :
        QMainWindow(parent),
        ui(new Ui::Client)
{
        _socket = socket;
        connect(_socket, SIGNAL(code4(json)), this, SLOT(code4(json)));
        connect(_socket, SIGNAL(code2(json)), this, SLOT(code2(json)));
        connect(this, SIGNAL(getRegisterNums()), _socket, SLOT(getRegisterNums()));
        connect(this, SIGNAL(getOnile()), _socket, SLOT(getOnile()));
        timer = new QTimer(this);
        //关联定时器溢出信号和相应的槽函数
        connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
        timer->start(2000);
        ui->setupUi(this);

}

Client::~Client() {
    delete ui;
}

void Client::code4(json _json){
    int nums =_json["nums"];
    qDebug()<<nums;
    string num = to_string(nums);
    ui->label_7->setText(QString(num.c_str()));
}

void Client::code2(json _json){
    ui->listWidget->clear();
    int nums =_json["nums"];
    auto all = _json["data"];
    qDebug()<<nums;
    string num = to_string(nums);
     ui->label_8->setText(QString(num.c_str()));
    for(auto i:all){
        ui->listWidget->addItem(QString::fromStdString(string(i)));
    }
}
void Client::timerUpdate(){
    emit getOnile();
    emit getRegisterNums();
}

void Client::on_pushButton_clicked()
{


}
