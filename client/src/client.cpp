#include "../include/client.hpp"
#include <QMessageBox>

Client::~Client() = default;

Client::Client(const std::string &ip, int port, QWidget *parent)
    : QMainWindow(parent), socket(new QTcpSocket(this))
{
    socket->connectToHost(QString::fromStdString(ip), port); // cast to correct string when we need it

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error",
                              "Failed to connect to server.");
    }
}
