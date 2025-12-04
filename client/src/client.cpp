#include "../include/client.hpp"

Client::Client(const std::string& ip, int port, QWidget *parent) : QMainWindow(parent) {
    sockfd = socket_create();
    if (sockfd < 0) {
        QMessageBox::critical(this, "Error", "Failed to create socket.");
        return;
    }

    if (!socket_connect(sockfd, ip.c_str(), port)) {
        QMessageBox::critical(this, "Error", "Failed to connect to server.");
        socket_close(sockfd);
        return;
    }

    // Start receiver thread
    recvThread = thread_create(Client::recv_loop, this);
    thread_detach(recvThread);

    // Not best practice, but I'm throwing all of the client 
    // GUI code here for the sake of saving time

    //=======================================================
    // QT STRUCTURE
    //=======================================================
    // Gloabl styling
    resize(800, 600);

    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    //===================================
    // Login Screen
    //
    // TODO:
    // 1. Send username, or send anon
    // 2. Make it less chopped
    //===================================
    QWidget* loginScreen = new QWidget(this);
    setCentralWidget(loginScreen);

    // OUTER layout (handles centering)
    QVBoxLayout* outer = new QVBoxLayout(loginScreen);

    // push content toward center
    outer->addStretch(1);

    // INNER layout (your actual UI block)
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    // --- Site Label ---
    QLabel* label = new QLabel("SECURE CHATROOM");
    label->setObjectName("site-label");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // --- Username input ---
    QLineEdit* usernameInput = new QLineEdit();
    usernameInput->setObjectName("username-input");
    usernameInput->setPlaceholderText("Enter username");
    usernameInput->setFixedWidth(300);
    layout->addWidget(usernameInput, 0, Qt::AlignCenter);

    // --- Connect button ---
    QPushButton* connectBtn = new QPushButton("Connect");
    connectBtn->setObjectName("connect-button");
    connectBtn->setFixedWidth(100);
    layout->addWidget(connectBtn, 0, Qt::AlignCenter);
    connect(connectBtn, &QPushButton::clicked, [this]() {
        stack->setCurrentIndex(1);
    });

    // Add your actual UI to outer layout
    outer->addLayout(layout);

    // push content toward center (bottom)
    outer->addStretch(1);

    //===================================
    // Chatroom Screen
    //
    // TODO:
    // 1. Add nav bar with: name (left), shop button that displays token count (right) - onclick, next page (back button top left)
    // 2. Chat area - allow the user to send chats
    // 3. Bottom field for sending chats (enter or click)
    // 4. Make it less chopped
    //===================================
    chatScreen = new QWidget();
    QVBoxLayout* chatLayout = new QVBoxLayout(chatScreen);

    QLabel* chatLabel = new QLabel("You are connected!");
    chatLabel->setAlignment(Qt::AlignCenter);
    chatLayout->addWidget(chatLabel);

    //===================================
    // Theme Shop Screen
    //
    // TODO:
    // 1. Add display with components for each theme
    //===================================
    shopScreen = new QWidget();
    QVBoxLayout* shopLayout = new QVBoxLayout(shopScreen);

    QLabel* shopLabel = new QLabel("You are connected!");
    shopLabel->setAlignment(Qt::AlignCenter);
    shopLayout->addWidget(shopLabel);

    // More global stuff
    stack = new QStackedWidget();
    stack->addWidget(loginScreen);  // index 0
    stack->addWidget(chatScreen);   // index 1
    stack->addWidget(shopScreen);   // index 2
    setCentralWidget(stack);

    show();
}

Client::~Client() {
    socket_close(sockfd);
    socket_cleanup();
}

// Receive loop for other users' messages
void* Client::recv_loop(void* arg) {
    Client* self = (Client*)arg;

    char buffer[1024];

    while (true) {
        int bytes = socket_recv(self->sockfd, buffer, sizeof(buffer));

        if (bytes <= 0) {
            std::cout << "[CLIENT] Server disconnected\n";
            break;
        }

        buffer[bytes] = '\0';
        std::cout << "[SERVER] " << buffer << std::endl;

        // If needed, emit signals to update Qt UI
        // emit self->messageReceived(QString::fromUtf8(buffer));
    }

    return nullptr;
}
