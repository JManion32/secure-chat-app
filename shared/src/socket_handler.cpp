#include "../include/socket_handler.hpp"
#include <cstring>

SocketType socket_create() {
#ifdef _WIN32
    static bool initialized = false;
    if (!initialized) {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
        initialized = true;
    }
#endif

    return socket(AF_INET, SOCK_STREAM, 0);
}

bool socket_connect(SocketType sock, const char* ip, int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

#ifdef _WIN32
    InetPton(AF_INET, ip, &addr.sin_addr);
#else
    inet_pton(AF_INET, ip, &addr.sin_addr);
#endif

    return connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool socket_bind(SocketType sock, int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool socket_listen(SocketType sock) {
    return listen(sock, 10) == 0;
}

SocketType socket_accept(SocketType server) {
    sockaddr_in addr{};
#ifdef _WIN32
    int len = sizeof(addr);
#else
    socklen_t len = sizeof(addr);
#endif
    return accept(server, (sockaddr*)&addr, &len);
}

int socket_send(SocketType sock, const void* data, int len) {
#ifdef _WIN32
    return send(sock, (const char*)data, len, 0);
#else
    return send(sock, data, len, 0);
#endif
}

int socket_recv(SocketType sock, void* buffer, int len) {
#ifdef _WIN32
    return recv(sock, (char*)buffer, len, 0);
#else
    return recv(sock, buffer, len, 0);
#endif
}

void socket_close(SocketType sock) {
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
}

