#pragma once
#include <vector>
#include <string>
#include "../../shared/include/socket_handler.hpp"
#include "../../shared/include/thread_handler.hpp"
#include "../../shared/include/protocol.hpp"

// Forward declaration
struct ClientConnection;
void* client_thread(void* arg);

// Global list of connected clients
extern std::vector<ClientConnection> g_clients;
extern pthread_mutex_t g_clients_mutex;

struct ClientConnection {
    SocketType sockfd;
    std::string username;
    std::string token;
    int credits = 0;

    std::vector<bool> ownedThemes = std::vector<bool>(9, false);

};

void broadcastMessage(const Message& msg);
void removeClient(SocketType sock);

//====================================================
// AUTH REQUEST
//====================================================
void handleAuthRequest(SocketType client_fd, const std::string& username);
std::string generateToken();

//====================================================
// SEND CHAT
//====================================================
void handleChatSend(SocketType client_fd, const std::string& payload);

//====================================================
// PURCHASE REQUEST
//====================================================
void handlePurchaseRequest(SocketType client_fd, const std::string& payload);
