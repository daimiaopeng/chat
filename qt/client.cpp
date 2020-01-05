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
        connect(_socket, SIGNAL(code3(json)), this, SLOT(code3(json)));
        connect(this, SIGNAL(getRegisterNums()), _socket, SLOT(getRegisterNums()));
        connect(this, SIGNAL(getOnile()), _socket, SLOT(getOnile()));
        timer = new QTimer(this);
        //关联定时器溢出信号和相应的槽函数
        connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
        timer->start(2000);
        ui->setupUi(this);
        ui->textBrowser->setReadOnly(true);
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

void Client::code3(json _json){
    ui->textBrowser->moveCursor(QTextCursor::End);
    QString message = QString::fromStdString(_json["data"]["message"]);
    QString sender = QString::fromStdString(_json["sender"]);
    qDebug()<<"message: "<<message;


    QString mess = sender+": "+message+"\n";
    ui->textBrowser->append("<font color=\"#FF0000\">"+mess+"</font>");

}

void Client::code2(json _json){
    ui->listWidget->clear();
    int nums =_json["nums"];
    auto all = _json["data"];
    qDebug()<<nums;
    string num = to_string(nums);
     ui->label_8->setText(QString(num.c_str()));
    for(const auto &i:all){
        if(i == _socket->userName.toStdString()) continue;
        ui->listWidget->addItem(QString::fromStdString(string(i)));
    }
}
void Client::timerUpdate(){
    emit getOnile();
    emit getRegisterNums();
    ui->label_14->setText(_socket->userName);
}


void Client::on_pushButton_clicked()
{
    QString input_data = ui->textEdit->toPlainText();
    if(input_data=="") return;
    json _json;
    _json["token"] = _socket->token.toStdString();
    _json["code"] = 3;
    _json["receiver"] = _socket->receiver.toStdString();
    _json["data"] = input_data.toStdString();
    _socket->writeData(QString::fromStdString(_json.dump()));
    ui->textBrowser->moveCursor(QTextCursor::End);
    QString mess = "you: "+input_data+"\n";
    ui->textBrowser->append("<font color=\"#000000\">"+mess+"</font>");
//    ui->textBrowser->append("<p style=\"direction: rtl\">123</p>");

//    ui->textBrowser->insertPlainText(mess);

    ui->textEdit->clear();
}

void Client::on_listWidget_itemActivated(QListWidgetItem *item)
{
    _socket->receiver = item->text();
    ui->label_13->setText(_socket->receiver);
}
