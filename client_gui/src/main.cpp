#include <QApplication>
#include "../include/client.hpp"
#include "../include/themes.hpp"
#include <cstdlib>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // env-switch
    const std::string SERVER_IP = "127.0.0.1";
    //const std::string SERVER_IP = std::getenv("SERVER_IP");

    const int SERVER_PORT = 5000;

    QString initialStyle = MAIN_QSS + "\n";

    if (THEME_MAP.contains("dark"))
        initialStyle += THEME_MAP.value("dark");

    app.setStyleSheet(initialStyle);

    Client client(SERVER_IP, SERVER_PORT);

    return app.exec();
}
