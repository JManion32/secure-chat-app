#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"
#include "../../common/include/protocol.hpp"

std::string generateSecret() {
    return "secret";
}

class Person {
    std::string username;
    std::string secret;
public:
    Person() = default;
    Person(const std::string& name) : username(name) {}
    void setSecret(const std::string& s) { secret = s; }
    const std::string& getUsername() const { return username; }
    const std::string& getSecret() const { return secret; }
};

class MessageSystem {
    std::vector<Person> people;
    std::vector<std::string> chats;
public:
    void addPerson(Person p) {
        std::string secret = generateSecret();
        p.setSecret(secret);
        people.push_back(std::move(p));
    }

    std::vector<std::string> getChats() const {
        return chats;
    }

    void pushChat(const std::string& c) { chats.push_back(c); }
};

// Thread entry point for clients
void* client_thread(void* arg) {
    SocketType client_fd = *(SocketType*)arg;
    delete (SocketType*)arg;

    std::cout << "[SERVER] Client thread started" << std::endl;

    // We'll accumulate bytes here because TCP is stream-based and
    // a recv may return partial or multiple messages.
    std::vector<uint8_t> recv_buffer;
    recv_buffer.reserve(2048);

    constexpr size_t TEMP_BUF_SIZE = 1024;
    uint8_t temp[TEMP_BUF_SIZE];

    while (true) {
        int bytes = socket_recv(client_fd, reinterpret_cast<char*>(temp), TEMP_BUF_SIZE);
        if (bytes <= 0) {
            std::cout << "[SERVER] Client disconnected or recv error (" << bytes << ")" << std::endl;
            break;
        }

        // Append newly read data
        recv_buffer.insert(recv_buffer.end(), temp, temp + bytes);

        // Process all complete messages in the buffer
        while (true) {
            // Need at least 4 bytes to read length prefix
            if (recv_buffer.size() < 4) break;

            // Read the 4-byte length prefix (network byte order)
            uint32_t body_len_be = 0;
            std::memcpy(&body_len_be, recv_buffer.data(), 4);
            uint32_t body_len = ntohl(body_len_be);

            // Sanity check body_len
            if (body_len < 5) {
                std::cerr << "[SERVER] Invalid body_len received: " << body_len << ". Closing connection." << std::endl;
                goto disconnect;
            }

            // Full packet size = 4 (len prefix) + body_len
            size_t full_packet_size = 4 + static_cast<size_t>(body_len);
            if (recv_buffer.size() < full_packet_size) {
                // Not enough data yet
                break;
            }

            // We have a full message. Body starts at offset 4.
            const uint8_t* body_ptr = recv_buffer.data() + 4;

            Message curMessage;
            bool ok = Protocol::deserialize(body_ptr, body_len, curMessage);
            if (!ok) {
                std::cerr << "[SERVER] Failed to deserialize message" << std::endl;
            } else {
                // EXAMPLE MESSAGE HANDLING 

                // std::cout << "[SERVER] Received message type=" << static_cast<int>(curMessage.type)
                //           << " user=" << static_cast<int>(curMessage.user)
                //           << " payload_len=" << curMessage.payload.size() << "\n";
         
                // if (!curMessage.payload.empty()) {
                //     std::cout << "[CUR MESSAGE] " << curMessage.payload << "\n";
                // }


                // 
                // TODO: actual message handling goes here auth, respond, other backend stuff
            }

            // Echo raw back (send exactly the bytes we just received)
            int sent = socket_send(client_fd, reinterpret_cast<const char*>(recv_buffer.data()), (int)full_packet_size);
            (void)sent; // ignore for now; handle partial send in production

            // Remove processed bytes from recv_buffer
            recv_buffer.erase(recv_buffer.begin(), recv_buffer.begin() + full_packet_size);
        }
    }

disconnect:
    socket_close(client_fd);
    return nullptr;
}