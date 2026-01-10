#include "../include/client_thread.hpp"
#include "../include/server.hpp"
#include <iostream>
#include <cstring>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Handle update from client
void handleMessage(SocketType client_fd, const json& msg) {
    if (msg["type"] == "auth.request") {
        handleAuthRequest(client_fd, msg["payload"]);
    }
    else if (msg["type"] == "chat.request") {
        handleChatRequest(client_fd, msg["payload"]);
    }
    else if (msg["type"] == "purchase.request") {
        handlePurchaseRequest(client_fd, msg["payload"]);
    }
    else {
        std::cout << "[SERVER] Unknown MessageType: " << msg["type"] << std::endl;
    }
}

// recv loop for an individual client
void* client_thread(void* arg) {
    SocketType client_fd = *(SocketType*)arg;
    delete (SocketType*)arg;

    std::cout << "[SERVER] Client thread started" << std::endl;

    // Register client
    pthread_mutex_lock(&global_clients_mutex);
    global_clients.push_back(Client(client_fd, ""));
    pthread_mutex_unlock(&global_clients_mutex);

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

            if (body_len < 1 || body_len > 1024 * 1024) {
                std::cerr << "[SERVER] Invalid body length" << std::endl;
                goto disconnect;
            }

            size_t full_packet = 4 + body_len;
            if (recv_buffer.size() < full_packet)
                break;

            const char* json_ptr = reinterpret_cast<const char*>(recv_buffer.data() + 4);
            std::string json_str(json_ptr, body_len);

            try {
                json msg = json::parse(json_str);

                if (!msg.contains("type") || !msg["type"].is_string()) {
                    std::cerr << "[SERVER] Missing or invalid type field" << std::endl;
                }
                else if (!msg.contains("payload") || !msg["payload"].is_object()) {
                    std::cerr << "[SERVER] Missing or invalid payload field" << std::endl;
                }
                else {
                    std::cout << "[SERVER] Received message type: " << msg["type"] << std::endl;
                    handleMessage(client_fd, msg);
                }

            }
            catch (const std::exception& e) {
                std::cerr << "[SERVER] JSON parse error: " << e.what() << std::endl;
            }

            recv_buffer.erase(recv_buffer.begin(), recv_buffer.begin() + full_packet);
        }
    }

disconnect:
    removeClient(client_fd);
    socket_close(client_fd);
    return nullptr;
}
