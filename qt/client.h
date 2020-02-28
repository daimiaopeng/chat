#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include"json.hpp"
#include"socket.h"
#include <QMainWindow>
#include <QTimer>
#include<QListWidget>
#include<QStringListModel>
#include<QTextEdit>
#include <QMessageBox>
#include<QFileDialog>
#include<QProgressDialog>
#include<QProgressBar>
using json = nlohmann::json;

namespace Ui {
    class Client;
}

class Client : public QMainWindow {
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr, Socket *socket = nullptr);
    ~Client();
    signals:
           void getRegisterNums();
           void getOnile();
private slots:
    void code5(json _json);
    void code4(json _json);
    void code2(json _json);
    void code3(json _json);
    void on_pushButton_clicked();
    void timerUpdate();

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_pushButton_2_clicked();

private:
    QTimer *timer;
    Ui::Client *ui;
    Socket *_socket;
    int bufsize;
};

#endif // CLIENT_H
