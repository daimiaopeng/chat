#include "register.h"
#include "ui_register.h"


Register::Register(QWidget *parent, Socket *socket) :
        QDialog(parent),
        ui(new Ui::Register) {
    _socket = socket;
    QObject::connect(_socket, SIGNAL(code1(json)), this, SLOT(code1(json)));
    QObject::connect(this, SIGNAL(reg(QString, QString)), _socket, SLOT(reg(QString, QString)));
    ui->setupUi(this);
}


Register::~Register() {
    delete ui;
}

void Register::code1(json _json) {
    string message;
    try {
        message = _json["data"]["message"];
        isCode = _json["isCode"];
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
    if (isCode == 0) {
        QMessageBox::warning(this, tr("Waring"), message.c_str(), QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(this, tr("Waring"), tr("注册成功"), QMessageBox::Yes);
    exit();
    qDebug() << "set isCode:" << isCode;
}

void Register::exit() {
    this->close();
}

void Register::on_registerBut_clicked() {
    QString name = ui->user->text();
    QString passwd1 = ui->pwdLineEdit_2->text();
    QString passwd2 = ui->pwdLineEdit_3->text();
    if (name == "") {
        QMessageBox::warning(this, tr("Waring"), tr("用户名为空"), QMessageBox::Yes);
        return;
    }
    if (passwd1 == "" || passwd2 == "") {
        QMessageBox::warning(this, tr("Waring"), tr("密码为空"), QMessageBox::Yes);
        return;
    }
    if (passwd1 != passwd2) {
        QMessageBox::warning(this, tr("Waring"), tr("两次输入密码不一致"), QMessageBox::Yes);
        return;
    }
    emit reg(name, passwd1);
    qDebug() << "yes";

}
