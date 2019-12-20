#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include"json.hpp"
#include"socket.h"

using json = nlohmann::json;
namespace Ui {
    class Client;
}

class Client : public QDialog {
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr, Socket *socket = nullptr);
    ~Client();

private slots:
    void code4(json _json);
    void on_pushButton_clicked();

signals:
    void getRegisterNums();
private:
    Ui::Client *ui;
    Socket *_socket;
};

#endif // CLIENT_H
