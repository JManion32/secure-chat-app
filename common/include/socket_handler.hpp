#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET SocketType;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketType;
#endif

// Socket API wrapper
SocketType socket_create();
bool socket_connect(SocketType sock, const char* ip, int port);
bool socket_bind(SocketType sock, int port);
bool socket_listen(SocketType sock, int backlog = 10);
SocketType socket_accept(SocketType server);
int socket_send(SocketType sock, const void* data, int len);
int socket_recv(SocketType sock, void* buffer, int len);
void socket_cleanup();  // No-op on Linux, WSACleanup() on Windows
void socket_close(SocketType sock);
