#pragma once

#include <QMainWindow>
#include <string>

#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"

class Client : public QMainWindow {
    Q_OBJECT

public:
    Client(const std::string& ip, int port, QWidget *parent = nullptr);
    ~Client();

private:
    SocketType sockfd;
    ThreadType recvThread;

    static void* recv_loop(void* arg);  // background thread
};
