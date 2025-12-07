#include "../include/protocol.hpp"

std::vector<uint8_t> Protocol::serialize(const Message& m) {
    uint32_t body_len = 1 + static_cast<uint32_t>(m.payload.size());

    std::vector<uint8_t> out;
    out.reserve(4 + body_len);

    // length prefix
    uint32_t len_be = htonl(body_len);
    out.insert(out.end(),
               reinterpret_cast<uint8_t*>(&len_be),
               reinterpret_cast<uint8_t*>(&len_be) + 4);

    // 1-byte type
    out.push_back(static_cast<uint8_t>(m.type));

    // payload
    out.insert(out.end(), m.payload.begin(), m.payload.end());

    return out;
}

bool Protocol::deserialize(const uint8_t* body, size_t bytes, Message& out) {
    if (bytes < 1) return false;

    out.type = static_cast<MessageType>(body[0]);

    if (bytes > 1) {
        out.payload.assign(reinterpret_cast<const char*>(body + 1),
                           bytes - 1);
    } else {
        out.payload.clear();
    }

    return true;
}