#include "../include/protocol_qt.hpp"

namespace ProtocolQt {

QByteArray encodeQt(const Protocol::Message& msg) {
    auto raw = Protocol::encode(msg);
    return QByteArray(reinterpret_cast<const char*>(raw.data()), raw.size());
}

Protocol::Message decodeQt(const QByteArray& arr) {
    std::vector<uint8_t> raw(arr.begin(), arr.end());
    return Protocol::decode(raw);
}

}
