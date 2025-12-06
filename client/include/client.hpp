#include <iostream>
#include <string>

#include <QMainWindow>
#include <QStackedWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"

class Client : public QMainWindow {

public:
    Client(const std::string& ip, int port, QWidget *parent = nullptr);
    ~Client();

private:
    SocketType sockfd;
    ThreadType recvThread;

    static void* recv_loop(void* arg);  // background thread for receiving server updates

    QWidget* buildLoginScreen();
    QWidget* buildChatScreen();
    QWidget* buildShopScreen();
    void addMessage(const QString& text, bool fromSelf);

    QWidget* loginScreen;
    QWidget* chatScreen;
    QWidget* shopScreen;

    QScrollArea* scroll = nullptr;
    QWidget* scrollContent = nullptr;
    QVBoxLayout* messageList = nullptr;


    QStackedWidget* stack = new QStackedWidget(this);
};
