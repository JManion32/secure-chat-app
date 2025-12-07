#include <iostream>
#include <string>

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>

#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"

class Client : public QMainWindow {

public:
    Client(const std::string& ip, int port, QWidget *parent = nullptr);
    ~Client();

private:
    SocketType sockfd;
    ThreadType recvThread;

    QString username;
    int credit_count;

    static void* recv_loop(void* arg);  // background thread for receiving server updates

    QWidget* buildLoginScreen();
    QWidget* buildChatScreen();
    QWidget* buildShopScreen();
    void addMessage(const QString& text, bool fromSelf);
    void applyTheme(const QString& themePath);

    QWidget* loginScreen;
    QWidget* chatScreen;
    QWidget* shopScreen;

    QScrollArea* scroll = nullptr;
    QWidget* scrollContent = nullptr;
    QVBoxLayout* messageList = nullptr;


    QStackedWidget* stack = new QStackedWidget(this);
};
