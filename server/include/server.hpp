#pragma once
#include <vector>
#include <string>

#include "./models/client.hpp"

#include "../../shared/include/socket_handler.hpp"
#include "../../shared/include/thread_handler.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void* client_thread(void* arg);

// Global list of connected clients
extern std::vector<Client> global_clients;
extern pthread_mutex_t global_clients_mutex;
extern int active_clients;

void broadcastMessage(const json& msg);
void removeClient(SocketType sock);

//====================================================
// AUTH REQUEST
//====================================================
void handleAuthRequest(SocketType client_fd, const json& payload);
std::string generateToken();

//====================================================
// SEND CHAT
//====================================================
void handleChatRequest(SocketType client_fd, const json& payload);

//====================================================
// PURCHASE REQUEST
//====================================================
void handlePurchaseRequest(SocketType client_fd, const json& payload);
