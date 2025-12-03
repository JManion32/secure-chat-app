// main.cpp
#include <QApplication>
#include "client.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const std::string SERVER_IP = "165.227.182.111";
    const int SERVER_PORT = 5000;

    Client client(SERVER_IP, SERVER_PORT);
    client.show();

    return app.exec();
}
