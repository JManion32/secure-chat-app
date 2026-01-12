#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <unistd.h>

#include "../../shared/include/socket_handler.hpp"

class Client {
public:
    // Constructors
    Client(SocketType sockfd, std::string name, std::string token);

    // Setters
    void setSockfd(SocketType sockfd) { this->sockfd = sockfd; }
    void setName(std::string name) { this->name = name; }
    void setToken(std::string token) { this->token = token; }
    void setCredits(int credits) { this->credits = credits; }
    void ownTheme(int index) { owned_themes[index] = true; }
    void generateToken() { this->token = generateRandomToken(); }

    // Getters
    SocketType getSockfd() const { return sockfd; }
    std::string getName() const { return name; }
    std::string getToken() const { return token; }
    int getCredits() const { return credits; }
    bool isThemeOwned(int index) { return owned_themes[index]; }

    // Member Functions
    void incrementCredits() { credits++; }
    void subtractPrice(int price) { credits = credits - price; }

private:
    SocketType sockfd;
    std::string name;
    std::string token;
    int credits = 0;
    std::vector<bool> owned_themes = std::vector<bool>(9, false);

    std::string generateRandomToken();
};