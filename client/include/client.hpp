#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

class Client : public QMainWindow {
public:
    Client(const std::string &ip, int port, QWidget *parent = nullptr);
    ~Client();

private:
    QTcpSocket *socket;
};

#endif