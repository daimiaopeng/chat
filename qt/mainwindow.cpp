#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent, Socket *socket,Client *c)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _socket = socket;
    _c = c;
    QObject::connect(_socket, SIGNAL(code0(json)), this, SLOT(code0(json)));
    QObject::connect(this, SIGNAL(login(QString, QString)), _socket, SLOT(login(QString, QString)));
    connect(_socket->tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    //connect(_socket->tcpSocket, SIGNAL(disconnected()), this, SLOT(linkError()));
    ui->usrLineEdit->setFocus();
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::linkError(){
    QMessageBox::warning(this, tr("Waring"), tr("连接失败"), QMessageBox::Yes);
}
void MainWindow::code0(json _json) {
    try {
        string message = _json["data"]["message"];
        token = QString::fromStdString(_json["token"]);
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
    _socket->token = token;
    qDebug() << "set token:" << token;
    goCilent( _json);
}

void MainWindow::goCilent(json jsonMessage) {
    string message = jsonMessage["data"]["message"];
    if (token != "") {
        QMessageBox::warning(this, tr("Waring"), message.c_str(), QMessageBox::Yes);
    } else {
        ui->pwdLineEdit->clear();
        ui->pwdLineEdit->setFocus();
        QMessageBox::warning(this, tr("Waring"), message.c_str(), QMessageBox::Yes);
    }
}

void MainWindow::on_exitBtn_clicked() {
//    accept();
}


void MainWindow::on_loginBtn_clicked() {
    QString name = ui->usrLineEdit->text();
    QString passwd = ui->pwdLineEdit->text();
    if (name == "") {
        QMessageBox::warning(this, tr("Waring"), tr("用户名为空"), QMessageBox::Yes);
        return;
    }
    if (passwd == "") {
        QMessageBox::warning(this, tr("Waring"), tr("密码为空"), QMessageBox::Yes);
        return;
    }
    emit login(name, passwd);
}

void MainWindow::connected(){
    QMessageBox::warning(this, tr("Waring"), tr("连接成功"), QMessageBox::Yes);
}
void MainWindow::on_registered_clicked() {
    Register reg(nullptr, _socket);
    reg.exec();
    qDebug() << "yes";
}

void MainWindow::on_linkBtn_clicked()
{
    QString ip = ui->ipLineEdit->text();
    int port = ui->portLineEdit->text().toInt();
    _socket->init(ip,port);
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
    _c->show();

}
