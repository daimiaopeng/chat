#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <socket.h>
#include "mainwindow.h"
#include <QMessageBox>

namespace Ui {
    class Register;
}

class Register : public QDialog {
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr, Socket *socket = nullptr);

    ~Register();


    signals:

    void  reg(QString name,QString passwd);


private slots:

    void on_registerBut_clicked();

    void code1(json _json);

private:
    void exit();

    int isCode;
    Ui::Register *ui;
    Socket *_socket;
};

#endif // REGISTER_H
