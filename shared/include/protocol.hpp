#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
  #include <winsock2.h>
#else
  #include <arpa/inet.h>
#endif

enum class MessageType : uint8_t {
    AUTH_REQUEST = 0,
    AUTH_RESPONSE = 1,
    CHAT_SEND = 2,
    CHAT_DELIVER = 3,
    PURCHASE_REQUEST = 4,
    PURCHASE_RESPONSE = 5,
};

struct Message {
    MessageType type;
    std::string payload;
};

class Protocol {
public:
    static std::vector<uint8_t> serialize(const Message& m);
    static bool deserialize(const uint8_t* body, size_t bytes, Message& out);
};
