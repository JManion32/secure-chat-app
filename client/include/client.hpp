#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

class Client : public QMainWindow {
    Q_OBJECT

public:
    Client(const QString &ip, quint16 port, QWidget *parent = nullptr);
    ~Client() override = default;

private:
    QTcpSocket *socket;
};

#endif
