#include "../include/server.hpp"
#include <algorithm>

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
