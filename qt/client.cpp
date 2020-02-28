#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent, Socket *socket) :
        QMainWindow(parent),
        ui(new Ui::Client)
{
        _socket = socket;
        connect(_socket, SIGNAL(code5(json)), this, SLOT(code5(json)));
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
        setWindowTitle(tr("聊天客户端"));
        bufsize = 3000;
}

Client::~Client() {
    delete ui;
}

void Client::code5(json _json){
    qDebug()<<"code5";
    int size =_json["size"];
    int part = _json["part"];
    if(ui->progressBar->value()==0){
        ui->progressBar->setRange(0,size);
    }
    ui->progressBar->setValue(part);
    if(ui->progressBar->value()==100){
        QMessageBox::warning(this, tr("Waring"), "接收文件成功", QMessageBox::Yes);
        ui->progressBar->setValue(0);
    }
    QFile file1("test.png");
    file1.open(QIODevice::Append|QIODevice::ReadWrite);
    QDataStream out(&file1);
    out.setVersion(QDataStream::Qt_5_7);
    QByteArray data = QByteArray::fromBase64(string(_json["data"]["message"]).c_str(), QByteArray::Base64Encoding);
    out.writeRawData(data,data.size());
    file1.close();
}

void Client::code4(json _json){
    int nums =_json["nums"];
    qDebug()<<nums;
    string num = to_string(nums);
    ui->label_7->setText(QString(num.c_str()));
}

void Client::code3(json _json){
    ui->textBrowser->moveCursor(QTextCursor::End);
    QString message = QString::fromStdString(_json["data"]["message"]).toLocal8Bit();
    QString sender = QString::fromStdString(_json["sender"]);
    qDebug()<<"message: "<<message;


    QString mess = "<font color=\"#FF0000\">"+sender+": "+message+"\n"+"</font>";

    ui->textBrowser->append(mess);

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
//    emit getRegisterNums();
    ui->label_14->setText(_socket->userName);
}


void Client::on_pushButton_clicked()
{
    QString input_data = ui->textEdit->toPlainText();
    if(input_data=="") return;
    if(_socket->receiver==""){
        QMessageBox::warning(this, tr("Waring"), "请选择一个聊天用户", QMessageBox::Yes);
        return;
    }
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

void Client::on_pushButton_2_clicked()
{


        QString curPath=QDir::currentPath();//获取系统当前目录
        //获取应用程序的路径
        QString dlgTitle="选择一个文件"; //对话框标题
        QString filter="所有文件(*.*)"; //文件过滤器
//        QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"; //文件过滤器
        QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
        QProgressDialog process(this);
        if (!aFileName.isEmpty()){
            QFile file(aFileName);
            file.open(QIODevice::ReadOnly);
            QByteArray fileData = file.readAll();
            string fileDataString(fileData.toBase64().toStdString());

            qDebug()<<"fileData.size()"<<fileData.size();
            qDebug()<<"fileData string size"<<fileDataString.size();

            process.setLabelText(tr("processing..."));
            process.setRange(0,fileData.size());
            process.setModal(true);
            process.setCancelButtonText(tr("cancel"));

            json _json;
            _json["token"] = _socket->token.toStdString();
            _json["code"] = 5;
            _json["size"] = fileDataString.size();
            _json["receiver"] = _socket->receiver.toStdString();
            for(long long i=0;i<fileDataString.size();i = i+bufsize){
                _json["part"] = i;
                _json["data"] = fileDataString.substr(i,bufsize);
                _socket->writeData(QString::fromStdString(_json.dump()));
                process.setValue(i);
            }
            file.close();
        }

}
