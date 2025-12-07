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

    resize(800, 600);

    loginScreen = buildLoginScreen();
    chatScreen  = buildChatScreen();
    shopScreen  = buildShopScreen();

    stack->addWidget(loginScreen);
    stack->addWidget(chatScreen);
    stack->addWidget(shopScreen);

    setCentralWidget(stack);

    show(); // show the GUI
}

Client::~Client() {
    socket_close(sockfd);
}

// Receive updates from the server
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


//===================================
// Login Screen
//===================================
QWidget* Client::buildLoginScreen() {
    QWidget* loginScreen = new QWidget();
    loginScreen->setObjectName("login-screen");


    QVBoxLayout* outer = new QVBoxLayout(loginScreen);
    outer->addStretch(1);

    QVBoxLayout* loginLayout = new QVBoxLayout();
    loginLayout->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("SECURE CHATROOM");
    titleLabel->setObjectName("site-label");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* descLabel = new QLabel("Send messages to unlock cool themes!");
    descLabel->setObjectName("site-desc");
    descLabel->setAlignment(Qt::AlignCenter);

    QLineEdit* usernameInput = new QLineEdit();
    usernameInput->setObjectName("username-input");
    usernameInput->setPlaceholderText("Enter username");
    usernameInput->setFixedWidth(300);

    QPushButton* connectButton = new QPushButton("Connect");
    connectButton->setObjectName("connect-button");
    connectButton->setFixedWidth(100);
    connect(connectButton, &QPushButton::clicked, [this]() {
        stack->setCurrentIndex(1);
    });

    loginLayout->addWidget(titleLabel);
    loginLayout->addWidget(descLabel);
    loginLayout->addWidget(usernameInput, 0, Qt::AlignCenter);
    loginLayout->addWidget(connectButton, 0, Qt::AlignCenter);

    outer->addLayout(loginLayout);
    outer->addStretch(1);

    return loginScreen;
}


//===================================
// Chat Build Screen
//===================================
QWidget* Client::buildChatScreen() {
    QWidget* chatScreen = new QWidget();
    chatScreen->setObjectName("chat-screen");

    // OUTER LAYOUT
    QVBoxLayout* outer = new QVBoxLayout(chatScreen);
    outer->setContentsMargins(20, 20, 20, 20);
    outer->setSpacing(10);

    //===================================
    // HEADER (top 10%)
    //===================================
    QWidget* headerWidget = new QWidget();
    headerWidget->setObjectName("chat-header");

    // Good fixed height for toolbar-like header
    headerWidget->setFixedHeight(60);

    // HEADER LAYOUT
    QHBoxLayout* header = new QHBoxLayout(headerWidget);
    header->setContentsMargins(15, 10, 15, 10);   // padding inside header
    header->setSpacing(10);

    // USERNAME LABEL
    QLabel* nameLabel = new QLabel("Justin");
    nameLabel->setObjectName("chat-username");
    nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    header->addWidget(nameLabel);

    // SPACE BETWEEN LEFT AND RIGHT
    header->addStretch(1);

    // SHOP BUTTON
    QPushButton* shopButton = new QPushButton("Credits: 100");
    shopButton->setObjectName("theme-shop-button");
    shopButton->setMinimumWidth(140);        // more flexible than fixedWidth
    shopButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(shopButton, &QPushButton::clicked, [this]() {
        stack->setCurrentIndex(2);
    });

    header->addWidget(shopButton);

    //===================================
    // SCROLLABLE MESSAGE AREA (middle)
    //===================================
    scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setObjectName("chat-scroll");

    scrollContent = new QWidget();
    scrollContent->setObjectName("chat-content");

    messageList = new QVBoxLayout(scrollContent);
    messageList->setAlignment(Qt::AlignTop);
    messageList->setSpacing(12);

    scroll->setWidget(scrollContent);

    //===================================
    // BOTTOM INPUT BAR (bottom 10%)
    //===================================
    QWidget* inputWidget = new QWidget();
    inputWidget->setObjectName("chat-input");
    inputWidget->setFixedHeight(60);

    // INPUT BAR LAYOUT
    QHBoxLayout* inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(15, 10, 15, 10);
    inputLayout->setSpacing(10);

    // MESSAGE BOX
    QLineEdit* messageBox = new QLineEdit();
    messageBox->setObjectName("message-input-box");
    messageBox->setPlaceholderText("Type a message…");
    messageBox->setMinimumWidth(200);
    messageBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // SEND BUTTON
    QPushButton* sendButton = new QPushButton("Send");
    sendButton->setObjectName("chat-send-button");
    sendButton->setMinimumWidth(100);
    sendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(sendButton, &QPushButton::clicked, [this]() {
        addMessage("New Message", true);
    });

    inputLayout->addWidget(messageBox);
    inputLayout->addWidget(sendButton);

    //===================================
    // ADD ALL SECTIONS TO MAIN LAYOUT
    //===================================
    outer->addWidget(headerWidget, 0);
    outer->addWidget(scroll, 1);     // middle gets all space
    outer->addWidget(inputWidget, 0);

    // DEBUG: Preload sample messages
    addMessage("Hello, welcome to the chat!", false);
    addMessage("Hey! This is what my own messages look like.", true);
    addMessage("Testing long messages to make sure wrapping works. "
            "This should take up multiple lines depending on the window size. "
            "Qt should wrap it correctly.", false);
    addMessage("Another message from me, right aligned!", true);
    addMessage("Short one.", false);
    addMessage("This is a longer message from myself that should align "
            "to the right and stay within 75% width.", true);
    addMessage("This is another message", false);

    return chatScreen;
}


//===================================
// Theme Shop Screen
//===================================
QWidget* Client::buildShopScreen() {
    QWidget* shopScreen = new QWidget();
    shopScreen->setObjectName("shop-screen");

    // OUTER LAYOUT (padding container)
    QVBoxLayout* outer = new QVBoxLayout(shopScreen);
    outer->setContentsMargins(30, 30, 30, 30);
    outer->setSpacing(20);

    // MAIN LAYOUT
    QVBoxLayout* shopLayout = new QVBoxLayout();
    shopLayout->setSpacing(20);

    //===================================
    // HEADER BAR
    //===================================
    QHBoxLayout* header = new QHBoxLayout();

    // Return button
    QPushButton* returnButton = new QPushButton("← Back");
    returnButton->setObjectName("shop-return-button");
    returnButton->setFixedWidth(120);
    connect(returnButton, &QPushButton::clicked, [this]() {
        stack->setCurrentIndex(1);
    });

    // Credits label
    QLabel* creditLabel = new QLabel("Credits: 100");
    creditLabel->setObjectName("shop-credit-label");
    creditLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    // Add to layout PROPERLY
    header->addWidget(returnButton);
    header->addStretch(1);       // IMPORTANT
    header->addWidget(creditLabel);

    // Wrap the header in a fixed-height widget
    QWidget* headerWidget = new QWidget();
    headerWidget->setObjectName("shop-header");
    headerWidget->setLayout(header);
    headerWidget->setFixedHeight(60);  // adjust height as needed
    headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    shopLayout->addWidget(headerWidget, 0);

    //===================================
    // GRID OF THEMES
    //===================================
    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(25);

    struct ThemeItem {
        QString name;
        QString imagePath;
        QString qssPath;
        int price;
    };

    std::vector<ThemeItem> themes = {
        
        {"Light Mode",   "../../client/themes/light_mode.png", "../../client/styles/theme-styles/light.qss",   0},

        {"Dark Mode",   "../../client/themes/dark_mode.png", "../../client/styles/theme-styles/black.qss",   0},

        {"Warm Tones",   "../../client/themes/warm_mode.png", "../../client/styles/theme-styles/warm.qss",    100},

        {"Neon Lights",  "../../client/themes/neon_mode.png", "../../client/styles/theme-styles/neon.qss",    100},

        {"Forest",       "../../client/themes/forest_mode.png", "../../client/styles/theme-styles/forest.qss",  300},

        {"Retro",        "../../client/themes/retro_mode.png", "../../client/styles/theme-styles/retro.qss",   300},

        {"Halloween",    "../../client/themes/halloween_mode.png", "../../client/styles/theme-styles/halloween.qss", 500},

        {"Festive",      "../../client/themes/festive_mode.png", "../../client/styles/theme-styles/festive.qss", 500},

        {"Premium Gold", "../../client/themes/premium_mode.png", "../../client/styles/theme-styles/premium.qss", 1000000},
    };

    const int columns = 3;  // scalable grid width

    for (int i = 0; i < (int)themes.size(); i++) {
        int row = i / columns;
        int col = i % columns;

        QWidget* card = new QWidget();
        card->setObjectName("theme-card");

        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setAlignment(Qt::AlignCenter);

        QLabel* img = new QLabel();
        img->setObjectName("theme-image");
        img->setAlignment(Qt::AlignCenter);

        // Fix the widget size
        img->setFixedHeight(50);

        QPixmap pix(themes[i].imagePath);
        img->setPixmap(
            pix.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

        QLabel* title = new QLabel(themes[i].name);
        title->setObjectName("theme-title");
        title->setAlignment(Qt::AlignCenter);

        QPushButton* buyBtn = new QPushButton("Buy (" + QString::number(themes[i].price) + ")");
        buyBtn->setObjectName("theme-buy-button");
        buyBtn->setFixedWidth(120);

        connect(buyBtn, &QPushButton::clicked, [this, themes, i]() {
            qDebug() << "Purchased theme:" << themes[i].name;
            applyTheme(themes[i].qssPath);
        });


        cardLayout->addWidget(img, 0, Qt::AlignCenter);
        cardLayout->addWidget(title, 0, Qt::AlignCenter);
        cardLayout->addWidget(buyBtn, 0, Qt::AlignCenter);

        grid->addWidget(card, row, col);
    }

    shopLayout->addLayout(grid);

    // add to outer
    outer->addLayout(shopLayout);

    return shopScreen;
}

void Client::addMessage(const QString& text, bool fromSelf) {
    // 1. MESSAGE BUBBLE (only contains the actual text)
    QLabel* msg = new QLabel(text);
    msg->setWordWrap(true);
    msg->setObjectName(fromSelf ? "message-self" : "message-other");

    QWidget* bubble = new QWidget();
    bubble->setObjectName(fromSelf ? "bubble-self" : "bubble-other");

    int bubbleWidth = static_cast<int>(scroll->viewport()->width() * 0.75);
    bubble->setFixedWidth(bubbleWidth);

    QVBoxLayout* bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setContentsMargins(12, 8, 12, 8);
    bubbleLayout->addWidget(msg);

    // 2. METADATA ROW (UNDER the bubble)
    QString senderName = fromSelf ? "You" : "Justin";  
    QString timestamp  = QDateTime::currentDateTime().toString("hh:mm AP");

    QLabel* meta = new QLabel(senderName + " • " + timestamp);
    meta->setObjectName(fromSelf ? "message-meta-self" : "message-meta-other");
    meta->setWordWrap(false);

    meta->setAlignment(fromSelf ? Qt::AlignRight : Qt::AlignLeft);

    // 3. ROW FOR ALIGNMENT (bubble + metadata vertically)
    QWidget* msgBlock = new QWidget();
    QVBoxLayout* msgBlockLayout = new QVBoxLayout(msgBlock);
    msgBlockLayout->setSpacing(2);
    msgBlockLayout->setContentsMargins(0, 0, 0, 0);
    msgBlockLayout->addWidget(bubble);
    msgBlockLayout->addWidget(meta);


    // 4. OUTER ROW (left-align or right-align whole block)
    QWidget* row = new QWidget();
    QHBoxLayout* rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    if (fromSelf) {
        rowLayout->addStretch(1);
        rowLayout->addWidget(msgBlock);
    } else {
        rowLayout->addWidget(msgBlock);
        rowLayout->addStretch(1);
    }

    messageList->addWidget(row);

    // 5. AUTO SCROLL to bottom
    QTimer::singleShot(0, this, [this]() {
        QTimer::singleShot(0, this, [this]() {
            scroll->verticalScrollBar()->setValue(
                scroll->verticalScrollBar()->maximum()
            );
        });
    });
}

void Client::applyTheme(const QString& themePath) {
    QString style;

    QFile mainFile("../../client/styles/main.qss");
    if (mainFile.open(QFile::ReadOnly)) {
        style += QString::fromUtf8(mainFile.readAll()) + "\n";
    }

    QFile themeFile(themePath);
    if (themeFile.open(QFile::ReadOnly)) {
        style += QString::fromUtf8(themeFile.readAll()) + "\n";
    }

    // Apply to this window + all child widgets
    this->setStyleSheet(style);
}

