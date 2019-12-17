#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "client.h"


MainWindow::MainWindow(QWidget *parent, Socket *socket)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    _socket = socket;
    QObject::connect(_socket, SIGNAL(code0(json)), this, SLOT(code0(json)));
    QObject::connect(this, SIGNAL(login(QString, QString)), _socket, SLOT(login(QString, QString)));
    ui->setupUi(this);
    ui->usrLineEdit->setFocus();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::code0(json _json) {
    try {
        string message = _json["data"]["message"];
        token = QString::fromStdString(_json["token"]);
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
    goCilent(_json);
    qDebug() << "set token:" << token;
}

void MainWindow::goCilent(json jsonMessage) {
    string message = jsonMessage["data"]["message"];
    if (token != "") {
        Client client;
        QMessageBox::warning(this, tr("Waring"), message.c_str(), QMessageBox::Yes);
        this->close();
        client.exec();
        qDebug() << "yes";
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

void MainWindow::on_registered_clicked() {
    Register reg(nullptr, _socket);

    reg.exec();
    qDebug() << "yes";
}
