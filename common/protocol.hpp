#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace Protocol {

    enum class MessageType : uint8_t {
        LOGIN = 1,
        LOGIN_OK = 2,
        LOGIN_FAIL = 3,
        CHAT = 4,
        CHAT_BROADCAST = 5
    };

    struct Message {
        MessageType type;
        std::string payload;
    };

    std::vector<uint8_t> encode(const Message& msg);
    Message decode(const std::vector<uint8_t>& data);

}

#endif