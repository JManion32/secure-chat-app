#include "../include/message_system.hpp"
#include <utility>

#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

//========================================================
// PERSON
//========================================================
Person::Person(const std::string& username) {
    this->username = username;
    this->token = generateToken();
    this->credit_count = 0;
}

const std::string& Person::getUsername() const {
    return username;
}

const std::string& Person::getToken() const {
    return token;
}

int Person::getCreditCount() {
    return credit_count;
}

bool Person::operator==(const Person& other) const {
    return token == other.token;
}


// Helper
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

//========================================================
// MESSAGE SYSTEM
//========================================================
void MessageSystem::addPerson(Person p) {
    people.push_back(p);
}

void MessageSystem::removePerson(const Person& p) {
    people.erase(std::remove(people.begin(), people.end(), p), people.end());
}