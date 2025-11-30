#include "../include/client.hpp"
#include "../include/protocol_qt.hpp"
#include <QMessageBox>

Client::Client(const QString &ip, quint16 port, QWidget *parent)
    : QMainWindow(parent),
      socket(new QTcpSocket(this))
{
    socket->connectToHost(ip, port);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error",
                              "Failed to connect to server.");
    }
}
