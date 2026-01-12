#include "../include/server.hpp"
#include <algorithm>
#include <string>
#include <iostream>

std::vector<Client> global_clients;
pthread_mutex_t global_clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int getActiveCount() {
    pthread_mutex_lock(&global_clients_mutex);
    int count = 0;
    for (const auto& c : global_clients) {
        if (!c.getToken().empty()) {
            count++;
        }
    }
    pthread_mutex_unlock(&global_clients_mutex);
    return count;
}

// Send message to all connected clients
void broadcastMessage(const json& response) {
    std::vector<SocketType> sockets;

    pthread_mutex_lock(&global_clients_mutex);
    for (const auto& c : global_clients) {
        if (c.getToken() != "") {
            sockets.push_back(c.getSockfd());
        }
    }
    pthread_mutex_unlock(&global_clients_mutex);

    std::string out = response.dump();
    for (SocketType sock : sockets) {
        sendFrame(sock, out);
    }
}

// When a client disconnects
void removeClient(SocketType sock) {
    std::string name;
    bool wasAuthorized = false;

    pthread_mutex_lock(&global_clients_mutex);

    auto it = std::find_if(
        global_clients.begin(),
        global_clients.end(),
        [sock](const Client& c) {
            return c.getSockfd() == sock;
        }
    );

    if (it == global_clients.end()) {
        pthread_mutex_unlock(&global_clients_mutex);
        return;
    }

    wasAuthorized = !it->getToken().empty();
    name = it->getName();
    global_clients.erase(it);

    pthread_mutex_unlock(&global_clients_mutex);

    if (!wasAuthorized) {
        return;
    }

    std::string name_msg = name + " left the chat";
    json response = {
        {"type", "chat.response"},
        {"payload", {
            {"server", "true"},
            {"name", name},
            {"content", name_msg},
            {"activeCount", getActiveCount()}
        }}
    };
    broadcastMessage(response);
}

Client* getClientByFD(SocketType fd) {
    for (auto& c : global_clients) {
        if (c.getSockfd() == fd)
            return &c;
    }
    return nullptr;
}

//====================================================
// AUTH REQUEST
//====================================================
void handleAuthRequest(SocketType client_fd, const json& payload) {
    // Validate payload
    if (!payload.contains("name") || !payload["name"].is_string()) {
        std::cerr << "[AUTH] ERROR: missing or invalid username" << std::endl;
        return;
    }

    pthread_mutex_lock(&global_clients_mutex);

    Client* client = getClientByFD(client_fd);
    if (!client) {
            pthread_mutex_unlock(&global_clients_mutex);
            std::cerr << "[AUTH] ERROR: Client not found" << std::endl;
            return;
    }

    for (Client client : global_clients) {
        if (client.getName() == payload["name"]) {
            std::cout << client.getName() << " === " << payload["name"] << std::endl;
            pthread_mutex_unlock(&global_clients_mutex);
            std::cerr << "[AUTH] ERROR: Client name already used" << std::endl;
            json response = {
                {"type", "auth.response"},
                {"payload", {
                    {"success", "false"},
                    {"name", payload["name"]},
                    {"token", ""}
                }}
            };
            std::string out = response.dump();
            sendFrame(client_fd, out);
            return;
        }
    }

    client->setName(payload["name"]);
    client->generateToken();
    pthread_mutex_unlock(&global_clients_mutex);
    std::cout << "[AUTH] SUCCESS: user= " << client->getName() << " token= " << client->getToken() << std::endl;

    std::string name_msg = payload["name"].get<std::string>() + " joined the chat";
    
    json chat_response = {
        {"type", "chat.response"},
        {"payload", {
            {"server", "true"},
            {"name", payload["name"]},
            {"content", name_msg},
            {"activeCount", getActiveCount()}
        }}
    };

    broadcastMessage(chat_response);

    // Build AUTH_RESPONSE JSON
    json auth_response = {
        {"type", "auth.response"},
        {"payload", {
            {"success", "true"},
            {"name", client->getName()},
            {"token", client->getToken()}
            /*{"credits", client->getCredits()}*/
        }}
    };

    std::string out = auth_response.dump();
    sendFrame(client_fd, out);
}


//====================================================
// SEND CHAT
//====================================================
void handleChatRequest(SocketType client_fd, const json& payload) {

    pthread_mutex_lock(&global_clients_mutex);

    Client* client = getClientByFD(client_fd);
    if (!client) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[CHAT] ERROR: Client not found" << std::endl;
        return;
    }

    if (client->getToken() != payload["token"]) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[CHAT] ERROR: Invalid token" << std::endl;
        return;
    }

    /*
    if (client->getCredits() != payload["credits"]) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[CHAT] ERROR: Credit mismatch" << std::endl;
        return;
    }

    // OK — increase credits on server
    client->incrementCredits();
    */
    std::string username = client->getName();
    pthread_mutex_unlock(&global_clients_mutex);

    std::cout << "[CHAT] SUCCESS: from " << username << std::endl;

    std::string content = payload["content"];

    // Build CHAT_RESPONSE JSON
    json response = {
        {"type", "chat.response"},
        {"payload", {
            {"server", "false"},
            {"name", client->getName()},
            {"content", payload["content"]}
        }}
    };

    broadcastMessage(response);
}


//====================================================
// PURCHASE REQUEST
//====================================================
void handlePurchaseRequest(SocketType client_fd, const json& payload) {
    const int prices[9] = {0, 0, 25, 50, 75, 100, 150, 200, 100000};

    if (payload["index"] < 0 || payload["index"] >= 9) {
        std::cerr << "[PURCHASE] ERROR: Invalid itemIndex" << std::endl;
        return;
    }

    pthread_mutex_lock(&global_clients_mutex);

    Client* client = getClientByFD(client_fd);
    if (!client) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[PURCHASE] ERROR: Client not found" << std::endl;
        return;
    }

    // Validate token
    if (client->getToken() != payload["token"]) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[PURCHASE ERROR: Token mismatch" <<std::endl;
        return;
    }

    // Validate credits
    if (client->getCredits() != payload["credits"]) {
        pthread_mutex_unlock(&global_clients_mutex);
        std::cerr << "[PURCHASE] ERROR: Credit mismatch" << std::endl;
        return;
    }

    // Check if already owned
    if (client->isThemeOwned(payload["index"])) {
        pthread_mutex_unlock(&global_clients_mutex);
        json response = {
            {"type", "purchase.response"},
            {"payload", {
                {"success", false}
            }}
        };
        std::string out = response.dump();
        sendFrame(client_fd, out);
        return;
    }

    if (!payload.contains("index") || !payload["index"].is_number_integer()) {
        std::cerr << "[PURCHASE] ERROR: Invalid index" <<std::endl;
        return;
    }

    int index = payload["index"];

    if (index < 0 || index >= 9) {
        std::cerr << "[PURCHASE] ERROR: Index out of bounds" << std::endl;
        return;
    }

    if (client->getCredits() < prices[index]) {
        std::cerr << "[PURCHASE] ERROR: Not enough credits" << std::endl;
        return;
    }

    // Check affordability
    if (client->getCredits() < prices[index]) {
        pthread_mutex_unlock(&global_clients_mutex);
        json response = {
            {"type", "purchase.response"},
            {"payload", {
                {"success", false}
            }}
        };
        std::string out = response.dump();
        sendFrame(client_fd, out);
        return;
    }

    // SUCCESS: deduct credits and mark owned
    client->subtractPrice(prices[index]);
    client->ownTheme(payload["index"]);

    int newCredits = client->getCredits();

    pthread_mutex_unlock(&global_clients_mutex);

    std::cout << "[PURCHASE] SUCCESS: item=" << payload["index"] << " new credits=" << newCredits << std::endl;

    // SUCCESS PAYLOAD
    json response = {
        {"type", "purchase.response"},
        {"payload", {
            {"success", true},
            {"index", payload["index"]},
            {"credits", newCredits}
        }}
    };

    std::string out = response.dump();
    sendFrame(client_fd, out);
}
