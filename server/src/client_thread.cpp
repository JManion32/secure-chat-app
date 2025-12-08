#include "../include/client_thread.hpp"
#include "../include/server.hpp"
#include <iostream>
#include <cstring>

// Handle update from client
void handleMessage(SocketType client_fd, const Message& msg) {
    switch (msg.type) {

        case MessageType::AUTH_REQUEST:
            std::cout << "[SERVER] -> AUTH_REQUEST" << std::endl;
            handleAuthRequest(client_fd, msg.payload);
            break;

        case MessageType::CHAT_SEND:
            std::cout << "[SERVER] -> CHAT_SEND" << std::endl;
            handleChatSend(client_fd, msg.payload);
            break;

        case MessageType::PURCHASE_REQUEST:
            std::cout << "[SERVER] -> PURCHASE_REQUEST" << std::endl;
            handlePurchaseRequest(client_fd, msg.payload);
            break;

        default:
            std::cout << "[SERVER] Unknown MessageType: " << (int)msg.type << std::endl;
            break;
    }
}

// recv loop for an individual client
void* client_thread(void* arg) {
    SocketType client_fd = *(SocketType*)arg;
    delete (SocketType*)arg;

    std::cout << "[SERVER] Client thread started" << std::endl;

    // Register client
    pthread_mutex_lock(&g_clients_mutex);
    g_clients.push_back({ client_fd, "", "", 1 });
    pthread_mutex_unlock(&g_clients_mutex);

    std::vector<uint8_t> recv_buffer;
    recv_buffer.reserve(2048);

    uint8_t temp[1024];

    while (true) {
        int bytes = socket_recv(client_fd, reinterpret_cast<char*>(temp), sizeof(temp));
        if (bytes <= 0) {
            std::cout << "[SERVER] Client disconnected" << std::endl;
            break;
        }

        recv_buffer.insert(recv_buffer.end(), temp, temp + bytes);

        while (true) {
            if (recv_buffer.size() < 4)
                break;

            uint32_t body_len_be = 0;
            std::memcpy(&body_len_be, recv_buffer.data(), 4);
            uint32_t body_len = ntohl(body_len_be);

            if (body_len < 1) {
                std::cerr << "[SERVER] Invalid length" << std::endl;
                goto disconnect;
            }

            size_t full_packet = 4 + body_len;
            if (recv_buffer.size() < full_packet)
                break;

            const uint8_t* body_ptr = recv_buffer.data() + 4;

            Message msg;
            if (!Protocol::deserialize(body_ptr, body_len, msg)) {
                std::cerr << "[SERVER] Deserialization failed" << std::endl;
                goto next_packet;
            }
            handleMessage(client_fd, msg);
            std::cout << "[SERVER] Received message type: " << (int)msg.type << std::endl;

        next_packet:
            recv_buffer.erase(recv_buffer.begin(), recv_buffer.begin() + full_packet);
        }
    }

disconnect:
    removeClient(client_fd);
    socket_close(client_fd);
    return nullptr;
}
