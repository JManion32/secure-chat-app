#include "../include/server.hpp"
#include <algorithm>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>

std::vector<ClientConnection> g_clients;
pthread_mutex_t g_clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcastMessage(const Message& msg) {
    std::vector<uint8_t> data = Protocol::serialize(msg);

    pthread_mutex_lock(&g_clients_mutex);
    for (auto& c : g_clients) {
        socket_send(c.sockfd, reinterpret_cast<const char*>(data.data()), data.size());
    }
    pthread_mutex_unlock(&g_clients_mutex);
}

void removeClient(SocketType sock) {
    pthread_mutex_lock(&g_clients_mutex);
    g_clients.erase(
        std::remove_if(g_clients.begin(), g_clients.end(),
            [sock](const ClientConnection& c) { return c.sockfd == sock; }),
        g_clients.end()
    );
    pthread_mutex_unlock(&g_clients_mutex);
}

ClientConnection* getClientByFD(SocketType fd) {
    for (auto& c : g_clients) {
        if (c.sockfd == fd)
            return &c;
    }
    return nullptr;
}

//====================================================
// AUTH REQUEST
//====================================================
void handleAuthRequest(SocketType client_fd, const std::string& username) {
    pthread_mutex_lock(&g_clients_mutex);

    ClientConnection* c = getClientByFD(client_fd);
    if (!c) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] AUTH_REQUEST: Client not found\n";
        return;
    }

    c->username = username;
    c->token = generateToken();
    c->credits = 0;

    pthread_mutex_unlock(&g_clients_mutex);

    std::cout << "[SERVER] AUTH: user='" << username 
              << "' token=" << c->token << " credits=" << c->credits << "\n";

    // Build AUTH_RESPONSE
    Message resp;
    resp.type = MessageType::AUTH_RESPONSE;
    resp.payload = c->token;

    auto bytes = Protocol::serialize(resp);
    socket_send(client_fd, (const char*)bytes.data(), bytes.size());
}

std::string generateToken() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 255);

    std::ostringstream oss;
    for (int i = 0; i < 32; i++) {   // 32 bytes = 256-bit token
        int byte = dist(rd);
        oss << std::hex << std::setw(2) << std::setfill('0') << byte;
    }

    return oss.str();
}

//====================================================
// SEND CHAT
//====================================================
void handleChatSend(SocketType client_fd, const std::string& payload) {
    // Expected payload format:
    // "<token>|<credits>|<message>"

    size_t pos1 = payload.find('|');
    size_t pos2 = payload.find('|', pos1 + 1);

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        std::cerr << "[SERVER] CHAT_SEND: Invalid payload\n";
        return;
    }

    std::string token  = payload.substr(0, pos1);
    int clientCredits  = std::stoi(payload.substr(pos1 + 1, pos2 - pos1 - 1));
    std::string message = payload.substr(pos2 + 1);

    pthread_mutex_lock(&g_clients_mutex);

    ClientConnection* c = getClientByFD(client_fd);
    if (!c) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] CHAT_SEND: Client not found\n";
        return;
    }

    // Validate token
    if (c->token != token) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] CHAT_SEND: Invalid token\n";
        return;
    }

    // Validate credit count matches
    if (c->credits != clientCredits) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] CHAT_SEND: Credit mismatch\n";
        return;
    }

    // OK — increase credits on server
    c->credits += 1;

    std::string username = c->username;

    pthread_mutex_unlock(&g_clients_mutex);

    std::cout << "[SERVER] CHAT from " << username << ": " << message
              << " (new credits=" << c->credits << ")\n";

    // Build the message to broadcast
    Message outgoing;
    outgoing.type = MessageType::CHAT_DELIVER;
    outgoing.payload = c->token + "|" + username + "|" + message;

    broadcastMessage(outgoing);
}


//====================================================
// PURCHASE REQUEST
//====================================================
void handlePurchaseRequest(SocketType client_fd, const std::string& payload) {
    // Expected: "<token>|<credits>|<item_id>"
    std::vector<std::string> parts;
    size_t start = 0, end;

    while ((end = payload.find('|', start)) != std::string::npos) {
        parts.push_back(payload.substr(start, end - start));
        start = end + 1;
    }
    parts.push_back(payload.substr(start));

    if (parts.size() != 3) {
        std::cerr << "[SERVER] PURCHASE_REQUEST: Invalid payload format\n";
        return;
    }

    std::string token = parts[0];
    int clientCredits = std::stoi(parts[1]);
    int itemIndex     = std::stoi(parts[2]);

    // Prices (server authoritative)
    const int prices[9] = {0, 0, 25, 50, 75, 100, 150, 200, 100000};

    if (itemIndex < 0 || itemIndex >= 9) {
        std::cerr << "[SERVER] PURCHASE_REQUEST: Invalid itemIndex\n";
        return;
    }

    int price = prices[itemIndex];

    pthread_mutex_lock(&g_clients_mutex);

    ClientConnection* c = getClientByFD(client_fd);
    if (!c) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] PURCHASE: Client not found\n";
        return;
    }

    // Validate token
    if (c->token != token) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] PURCHASE: Token mismatch\n";
        return;
    }

    // Validate credits
    if (c->credits != clientCredits) {
        pthread_mutex_unlock(&g_clients_mutex);
        std::cerr << "[SERVER] PURCHASE: Credit mismatch\n";
        return;
    }

    // Check if already owned
    if (c->ownedThemes[itemIndex]) {
        pthread_mutex_unlock(&g_clients_mutex);

        Message resp;
        resp.type = MessageType::PURCHASE_RESPONSE;
        resp.payload = "NO|OWNED";

        auto data = Protocol::serialize(resp);
        socket_send(client_fd, (char*)data.data(), data.size());
        return;
    }

    // Check affordability
    if (c->credits < price) {
        pthread_mutex_unlock(&g_clients_mutex);

        Message resp;
        resp.type = MessageType::PURCHASE_RESPONSE;
        resp.payload = "NO|CREDITS";

        auto data = Protocol::serialize(resp);
        socket_send(client_fd, (char*)data.data(), data.size());
        return;
    }

    // SUCCESS: deduct credits and mark owned
    c->credits -= price;
    c->ownedThemes[itemIndex] = true;

    int newCredits = c->credits;

    pthread_mutex_unlock(&g_clients_mutex);

    std::cout << "[SERVER] PURCHASE OK: item=" << itemIndex
              << " new credits=" << newCredits << "\n";

    // SUCCESS PAYLOAD: YES|itemIndex|newCredits
    Message resp;
    resp.type = MessageType::PURCHASE_RESPONSE;
    resp.payload = "YES|" + std::to_string(itemIndex) + "|" + std::to_string(newCredits);

    auto data = Protocol::serialize(resp);
    socket_send(client_fd, (char*)data.data(), data.size());
}
