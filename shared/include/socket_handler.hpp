// Define a Socket API wrapper so we don't have to worry about 
// OS compatability in the rest of the code.

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

// Client
bool socket_connect(SocketType sock, const char* ip, int port);

// Server
SocketType socket_create();
bool socket_bind(SocketType sock, int port);
bool socket_listen(SocketType sock);
SocketType socket_accept(SocketType server);

//Both
int socket_recv(SocketType sock, void* buffer, int len);
int socket_send(SocketType sock, const void* data, int len);
void socket_close(SocketType sock);
