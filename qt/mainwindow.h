#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socket.h"
#include <QDebug>
#include<json.hpp>
#include "register.h"
#include "client.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Socket *socket = nullptr, Client *c = nullptr);

    ~MainWindow();

    signals:

            void
    login(QString
    name,
    QString passwd
    );

public
    slots:

    void on_exitBtn_clicked();

    void on_loginBtn_clicked();

    void code0(json _json);
    void linkError();
    void connected();
private
    slots:

            void

    on_registered_clicked();

            void on_linkBtn_clicked();

            void on_pushButton_clicked();

private:
    QString token;
    Socket *_socket;
    Client *_c;
    Ui::MainWindow *ui;
    bool w;
    void goCilent(json jsonMessage);
};

#endif // MAINWINDOW_H
