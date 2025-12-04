// main.cpp
#include <QApplication>
#include <QFile>
#include "client.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const std::string SERVER_IP = "165.227.182.111";
    const int SERVER_PORT = 5000;

    Client client(SERVER_IP, SERVER_PORT);
    client.show();

    QFile file("../../client/styles/main.qss");

    bool success = file.open(QFile::ReadOnly);
    if (!success) {
        std::cerr << "Cannot resolve main.qss" << std::endl;
    }

    QString style = QString::fromUtf8(file.readAll());
    app.setStyleSheet(style);

    return app.exec();
}
