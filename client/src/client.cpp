#include "../include/client.h"
#include <QMessageBox>
#include <QDebug>

Client::Client(const QString &ip, quint16 port, QWidget *parent)
    : QMainWindow(parent), socket(new QTcpSocket(this))
{
    socket->connectToHost(ip, port);

    if (socket->waitForConnected(5000)) {
        qDebug() << "Connected!";
    } else {
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

