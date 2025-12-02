#include "protocol.hpp"

namespace Protocol {

    /*
    // Format:
    // [type:1 byte] [payloadLen:2 bytes] [payload]

    std::vector<uint8_t> encode(const Message& msg) {
        std::vector<uint8_t> out;

        // Type
        out.push_back(static_cast<uint8_t>(msg.type));

        // Payload length
        uint16_t len = msg.payload.size();
        out.push_back(len >> 8);
        out.push_back(len & 0xFF);

        // Payload bytes
        out.insert(out.end(), msg.payload.begin(), msg.payload.end());

        return out;
    }

    Message decode(const std::vector<uint8_t>& data) {
        Message msg;

        size_t idx = 0;

        // Type
        msg.type = static_cast<MessageType>(data[idx++]);

        // Payload length
        uint16_t len = (data[idx] << 8) | data[idx + 1];
        idx += 2;

        // Payload string
        msg.payload.assign(data.begin() + idx,
                           data.begin() + idx + len);

        return msg;
    }
    */

}
