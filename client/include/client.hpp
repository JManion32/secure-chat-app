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
#include "../../common/include/protocol.hpp"

class Client : public QMainWindow {

public:
    Client(const std::string& ip, int port, QWidget *parent = nullptr);
    ~Client();

private:
    /*
        The client object has quite a few fields since it is both the client (person) 
        and the client (GUI). It has a background thread that runs the receiver socket, 
        as well as the client info (username, credits, token, etc), and all of the code for the GUI.
    */

    // Sockets and Threads
    SocketType sockfd;
    std::string ip;
    int port;
    ThreadType recvThread;
    static void* recv_loop(void* arg);  // background thread for receiving server updates
    void processIncomingMessage(const Message& msg);

    // Client Information
    QString username;
    QString token;
    int credit_count = 0;

    // GUI
    QWidget* buildLoginScreen();
    QWidget* buildChatScreen();
    QWidget* buildShopScreen();
    void addMessage(const QString& user, const QString& text, bool fromSelf);
    void applyTheme(const QString& themePath);

    QWidget* loginScreen;
    QWidget* chatScreen;
    QWidget* shopScreen;

    // These are the elements that change dynamically (excluding the chat area)
    QLabel* nameLabel;
    QPushButton* shopButton;
    QLabel* creditLabel;

    QScrollArea* scroll = nullptr;
    QWidget* scrollContent = nullptr;
    QVBoxLayout* messageList = nullptr;

    QStackedWidget* stack = new QStackedWidget(this);
};
