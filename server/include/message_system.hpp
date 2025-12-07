#pragma once
#include <string>
#include <vector>

//========================================================
// PERSON
//========================================================
class Person {
public:
    // Constructor
    Person(const std::string& username);

    // Getters
    const std::string& getUsername() const;
    const std::string& getToken() const;
    int getCreditCount();

    bool operator==(const Person& other) const;
private:
    // Class variables
    std::string username;
    std::string token;
    int credit_count;
};

// Helper
std::string generateToken();

//========================================================
// MESSAGE SYSTEM
//========================================================
class MessageSystem {
    std::vector<Person> people;
public:
    void addPerson(Person p);
    void removePerson(const Person& p);
};
