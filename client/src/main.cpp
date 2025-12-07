#include <QApplication>
#include <QFile>
#include "client.hpp"
#include <iostream>

static QString loadQSS(const QString &path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        std::cerr << "Failed to load QSS: " 
                  << path.toStdString() << std::endl;
        return "";
    }
    return QString::fromUtf8(file.readAll());
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const std::string SERVER_IP = "165.227.182.111";
    const int SERVER_PORT = 5000;

    QString style;

    style += loadQSS("../../client/styles/main.qss");
    style += "\n";
    style += loadQSS("../../client/styles/theme-styles/dark.qss");

    app.setStyleSheet(style);

    Client client(SERVER_IP, SERVER_PORT);

    return app.exec();
}
