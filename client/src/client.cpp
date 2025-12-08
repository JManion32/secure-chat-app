#include "../include/client.hpp"

Client::Client(const std::string& ip, int port, QWidget *parent) : QMainWindow(parent) {
    this->sockfd = socket_create();
    this->ip = ip.c_str();
    this->port = port;
    if (sockfd < 0) {
        QMessageBox::critical(this, "Error", "Failed to create socket.");
        return;
    }

    resize(800, 600);
    ownedThemes.resize(9, false);

    loginScreen = buildLoginScreen();
    chatScreen  = buildChatScreen();
    shopScreen  = buildShopScreen();

    stack->addWidget(loginScreen);
    stack->addWidget(chatScreen);
    stack->addWidget(shopScreen);

    setCentralWidget(stack);

    show();
}

Client::~Client() {
    socket_close(sockfd);
}

// Receive updates from the server
void* Client::recv_loop(void* arg) {
    Client* self = (Client*)arg;

    std::vector<uint8_t> recv_buffer;
    recv_buffer.reserve(2048);

    uint8_t temp[1024];

    while (true) {
        int bytes = socket_recv(self->sockfd, reinterpret_cast<char*>(temp), sizeof(temp));

        if (bytes <= 0) {
            std::cout << "[CLIENT] Server disconnected\n";
            break;
        }

        recv_buffer.insert(recv_buffer.end(), temp, temp + bytes);

        while (true) {
            if (recv_buffer.size() < 4)
                break;

            // Read the length prefix
            uint32_t body_len_be;
            std::memcpy(&body_len_be, recv_buffer.data(), 4);
            uint32_t body_len = ntohl(body_len_be);

            if (body_len < 1) {
                std::cerr << "[CLIENT] Invalid body length\n";
                goto disconnect;
            }

            size_t full_packet = 4 + body_len;

            if (recv_buffer.size() < full_packet)
                break;

            // Extract the message body
            const uint8_t* body_ptr = recv_buffer.data() + 4;

            Message msg;
            if (!Protocol::deserialize(body_ptr, body_len, msg)) {
                std::cerr << "[CLIENT] Failed to parse message\n";
                recv_buffer.erase(recv_buffer.begin(), recv_buffer.begin() + full_packet);
                continue;
            }

            // ----- Handle the message -----
            self->processIncomingMessage(msg);

            // Remove processed message
            recv_buffer.erase(recv_buffer.begin(), recv_buffer.begin() + full_packet);
        }
    }

disconnect:
    socket_close(self->sockfd);
    return nullptr;
}

// Process the incoming message from the server, and update the UI accordingly
void Client::processIncomingMessage(const Message& msg) {
    QMetaObject::invokeMethod(
        this,
        [this, msg]() {
            switch (msg.type) {
                case MessageType::AUTH_RESPONSE:
                    this->token = QString::fromStdString(msg.payload);
                    nameLabel->setText(username);
                    stack->setCurrentIndex(1);
                    break;

                case MessageType::CHAT_DELIVER: {
                    // Payload format: token|username|message
                    std::string p = msg.payload;
                    size_t pos1 = p.find('|');
                    size_t pos2 = p.find('|', pos1 + 1);

                    if (pos1 == std::string::npos || pos2 == std::string::npos)
                        return;

                    std::string senderToken  = p.substr(0, pos1);
                    std::string senderUser   = p.substr(pos1 + 1, pos2 - pos1 - 1);
                    std::string senderMsg    = p.substr(pos2 + 1);

                    bool fromSelf = (senderToken == token.toStdString());

                    QString qUser = QString::fromStdString(senderUser);
                    QString qMsg  = QString::fromStdString(senderMsg);

                    // Only increment local credits if WE sent this message
                    if (fromSelf) {
                        credit_count += 1;
                        shopButton->setText("Theme Shop (" + QString::number(credit_count) + ")");
                        if (creditLabel) {
                            creditLabel->setText("Credits: " + QString::number(credit_count));
                        }
                    }

                    addMessage(qUser, qMsg, fromSelf);
                    break;
                }

                case MessageType::PURCHASE_RESPONSE: {
                    QString r = QString::fromStdString(msg.payload);

                    if (r.startsWith("YES")) {
                        // Expected format: YES|<itemID>|<newCredits>
                        QStringList parts = r.split("|");
                        if (parts.size() >= 3) {
                            int itemID      = parts[1].toInt();
                            int newCredits  = parts[2].toInt();

                            credit_count = newCredits;
                            ownedThemes[itemID] = true;

                            if (shopButton) {
                                shopButton->setText(QString("Theme Shop (%1)").arg(credit_count));
                            }

                            if (creditLabel) {
                                creditLabel->setText("Credits: " + QString::number(credit_count));
                            }

                            if (itemID >= 0 && itemID < (int)themeButtons.size() && themeButtons[itemID]) {
                                themeButtons[itemID]->setText("Enable");
                            }

                            QMessageBox::information(this, "Purchase", "Theme unlocked!");
                        } else {
                            QMessageBox::warning(this, "Purchase", "Purchase response malformed.");
                        }
                    } else {
                        // NO or error message, don’t leave button permanently disabled
                        QMessageBox::warning(this, "Purchase", "Purchase failed: " + r);
                    }
                    break;
                }

                default:
                    break;
            }
        },
        Qt::QueuedConnection
    );
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

    // Title
    QLabel* titleLabel = new QLabel("SECURE CHATROOM");
    titleLabel->setObjectName("site-label");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Description
    QLabel* descLabel = new QLabel("Send messages to unlock cool themes!");
    descLabel->setObjectName("site-desc");
    descLabel->setAlignment(Qt::AlignCenter);

    // Username field
    QLineEdit* usernameInput = new QLineEdit();
    usernameInput->setObjectName("username-input");
    usernameInput->setPlaceholderText("Enter username");
    usernameInput->setFixedWidth(300);
    usernameInput->setMaxLength(32);

    // Connect button
    QPushButton* connectButton = new QPushButton("Connect");
    connectButton->setObjectName("connect-button");
    connectButton->setFixedWidth(100);

    // Start disabled
    connectButton->setEnabled(false);

    // Enable button only when there is text
    connect(usernameInput, &QLineEdit::textChanged, [connectButton](const QString& text) {
        connectButton->setEnabled(!text.trimmed().isEmpty());
    });

    connect(connectButton, &QPushButton::clicked, [this, usernameInput]() {         
        if (!socket_connect(sockfd, ip.c_str(), port)) {
            QMessageBox::critical(this, "Error", "Failed to connect to server.");
            socket_close(sockfd);
            return;
        }

        // Build AUTH_REQUEST
        Message authMsg;
        authMsg.type = MessageType::AUTH_REQUEST;

        this->username = usernameInput->text();
        authMsg.payload = username.toStdString();

        auto data = Protocol::serialize(authMsg);
        socket_send(sockfd, (const char*)data.data(), data.size());

        recvThread = thread_create(Client::recv_loop, this);
        thread_detach(recvThread);
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
    // HEADER
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
    nameLabel = new QLabel();
    nameLabel->setObjectName("chat-username");
    nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    header->addWidget(nameLabel);

    header->addStretch(1);

    // SHOP BUTTON
    shopButton = new QPushButton("Theme Shop (" + QString::number(credit_count) + ")");
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
    // BOTTOM INPUT BAR
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
    messageBox->setMaxLength(256);

    // SEND BUTTON
    QPushButton* sendButton = new QPushButton("Send");
    sendButton->setObjectName("chat-send-button");
    sendButton->setMinimumWidth(100);
    sendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // --- Only allow sending when messageBox has text ---
    sendButton->setEnabled(false);
    connect(messageBox, &QLineEdit::textChanged, [sendButton](const QString& text) {
        sendButton->setEnabled(!text.trimmed().isEmpty());
    });

    // --- Send button click handler ---
    connect(sendButton, &QPushButton::clicked, [this, messageBox]() {

        QString text = messageBox->text().trimmed();
        if (text.isEmpty()) return;

        // Build chat packet
        Message msg;
        msg.type = MessageType::CHAT_SEND;
        msg.payload =
            token.toStdString() + "|" +
            std::to_string(credit_count) + "|" +
            text.toStdString();

        // Serialize and send
        std::vector<uint8_t> data = Protocol::serialize(msg);
        socket_send(sockfd, (const char*)data.data(), data.size());

        // Clear input
        messageBox->clear();
    });

    connect(messageBox, &QLineEdit::returnPressed, sendButton, &QPushButton::click);

    // Auto-scroll to bottom after sending
    connect(sendButton, &QPushButton::clicked, [this]() {
        scroll->verticalScrollBar()->setValue(
            scroll->verticalScrollBar()->maximum()
        );
    });

    inputLayout->addWidget(messageBox);
    inputLayout->addWidget(sendButton);

    //===================================
    // ADD ALL SECTIONS TO MAIN LAYOUT
    //===================================
    outer->addWidget(headerWidget, 0);
    outer->addWidget(scroll, 1);     // middle gets all space
    outer->addWidget(inputWidget, 0);

    /*
    // DEBUG: PurchasedPreload sample messages
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
    */

    return chatScreen;
}

//===================================
// Theme Shop Build
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

    QPushButton* returnButton = new QPushButton("← Back");
    returnButton->setObjectName("shop-return-button");
    returnButton->setFixedWidth(120);
    connect(returnButton, &QPushButton::clicked, [this]() {
        stack->setCurrentIndex(1);
    });

    creditLabel = new QLabel("Credits: " + QString::number(credit_count));
    creditLabel->setObjectName("shop-credit-label");
    creditLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    header->addWidget(returnButton);
    header->addStretch(1);
    header->addWidget(creditLabel);

    QWidget* headerWidget = new QWidget();
    headerWidget->setObjectName("shop-header");
    headerWidget->setLayout(header);
    headerWidget->setFixedHeight(60);

    shopLayout->addWidget(headerWidget);

    //===================================
    // GRID OF THEMES
    //===================================

    themeButtons.resize(9, nullptr);

    struct ThemeItem {
        int id;
        QString name;
        QString imagePath;
        QString themeKey;
        int price;
    };

    std::vector<ThemeItem> themes = {
        {0, "Light Mode",   "../../client/themes/light_mode.png",    "light",     0},
        {1, "Dark Mode",    "../../client/themes/dark_mode.png",     "dark",      0},
        {2, "Warm Tones",   "../../client/themes/warm_mode.png",     "warm",     25},
        {3, "Neon Lights",  "../../client/themes/neon_mode.png",     "neon",     50},
        {4, "Forest",       "../../client/themes/forest_mode.png",   "forest",   75},
        {5, "Retro",        "../../client/themes/retro_mode.png",    "retro",    100},
        {6, "Halloween",    "../../client/themes/halloween_mode.png","halloween",150},
        {7, "Festive",      "../../client/themes/festive_mode.png",  "festive",  200},
        {8, "Premium Gold", "../../client/themes/premium_mode.png",  "premium", 100000},
    };

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(25);

    const int columns = 3;

    for (int i = 0; i < (int)themes.size(); i++) {

        int row = i / columns;
        int col = i % columns;

        const ThemeItem& theme = themes[i];
        int themeId = theme.id;

        QWidget* card = new QWidget();
        card->setObjectName("theme-card");

        // Make the card expand to fill the grid cell
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* cardLayout = new QVBoxLayout(card);

        cardLayout->setAlignment(Qt::AlignCenter);

        // Image
        QLabel* img = new QLabel();
        img->setObjectName("theme-image");
        img->setAlignment(Qt::AlignCenter);
        img->setFixedHeight(50);

        QPixmap pix(theme.imagePath);
        img->setPixmap(
            pix.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );

        // Title
        QLabel* title = new QLabel(theme.name);
        title->setObjectName("theme-title");
        title->setAlignment(Qt::AlignCenter);

        // Button
        QPushButton* buyBtn = new QPushButton();
        buyBtn->setObjectName("theme-buy-button");
        buyBtn->setFixedWidth(120);
        themeButtons[themeId] = buyBtn;

        // Button text depends on ownership
        if (ownedThemes[themeId]) {
            buyBtn->setText("Enable");
        } else {
            buyBtn->setText("Buy (" + QString::number(theme.price) + ")");
        }

        // Handle click
        connect(buyBtn, &QPushButton::clicked, [this, themeId, theme]() {

            if (ownedThemes[themeId]) {
                // Already bought → just apply theme
                applyTheme(theme.themeKey);
                return;
            }

            // Not owned, send purchase request
            Message msg;
            msg.type = MessageType::PURCHASE_REQUEST;
            msg.payload =
                token.toStdString() + "|" +
                std::to_string(credit_count) + "|" +
                std::to_string(themeId);

            auto data = Protocol::serialize(msg);
            socket_send(sockfd, (char*)data.data(), data.size());
        });

        // Add widgets
        cardLayout->addWidget(img, 0, Qt::AlignHCenter);
        cardLayout->addWidget(title, 0, Qt::AlignHCenter);
        cardLayout->addWidget(buyBtn, 0, Qt::AlignHCenter);

        // Push everything to the top visually
        //cardLayout->addStretch();

        grid->addWidget(card, row, col);
    }

    shopLayout->addLayout(grid);
    outer->addLayout(shopLayout);

    return shopScreen;
}

// Add a message to the chat area
void Client::addMessage(const QString& user, const QString& text, bool fromSelf) {
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
    QString senderName = fromSelf ? "You" : user;  
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

// Apply the chosen theme
void Client::applyTheme(const QString& themeName) {
    QString style;

    // Base (site-wide) styling:
    style += MAIN_QSS + "\n";

    // Theme-specific styling:
    if (THEME_MAP.contains(themeName)) {
        style += THEME_MAP.value(themeName);
    }

    this->setStyleSheet(style);
}
