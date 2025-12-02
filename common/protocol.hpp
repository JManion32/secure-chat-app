#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace Protocol {

    /*
    enum class MessageType : uint8_t {
        // User selecting nickname, getting token
        LOGIN = 1,
        LOGIN_OK = 2,
        LOGIN_FAIL = 3,
        // Sending chat, verifying chat, adding to credit count
        SEND_CHAT = 4,
        CHAT_OK = 5,
        CHAT_FAIL = 6,
        // User purchasing themes with their credits
        PURCHASE = 7,
        PURCHASE_OK = 8,
        PURCHASE_FAIL = 9
    };

    
    struct Message {
        MessageType type;
        std::string payload;
    };

    std::vector<uint8_t> encode(const Message& msg);
    Message decode(const std::vector<uint8_t>& data);
    */

}

#endif