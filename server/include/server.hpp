#pragma once
#include <vector>
#include <string>
#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"
#include "../../common/include/protocol.hpp"

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
};

// Helpers
void broadcastMessage(const Message& msg);
void removeClient(SocketType sock);
