#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include"json.hpp"
#include"socket.h"
#include <QMainWindow>
#include <QTimer>

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
    void code4(json _json);
    void code2(json _json);
    void on_pushButton_clicked();
    void timerUpdate();

private:
    QTimer *timer;
    Ui::Client *ui;
    Socket *_socket;
};

#endif // CLIENT_H
