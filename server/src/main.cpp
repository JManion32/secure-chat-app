#include <iostream>
#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"

static const int SERVER_PORT = 5000;

// Thread entry point for clients
void* client_thread(void* arg) {
    SocketType client_fd = *(SocketType*)arg;
    delete (SocketType*)arg;

    std::cout << "[SERVER] Client thread started\n";

    // Handle client here
    // e.g. read, write, etc.
    // socket_recv(client_fd, buffer, ...)

    SocketClose(client_fd);
    return nullptr;
}

int main() {
    std::cout << "[SERVER] Starting server...\n";

    // 1. Create server socket
    SocketType server_fd = socket_create();
    if (server_fd < 0) {
        std::cerr << "[SERVER] socket_create() failed\n";
        return -1;
    }

    // 2. Bind to port
    if (!socket_bind(server_fd, SERVER_PORT)) {
        std::cerr << "[SERVER] bind() failed\n";
        return -1;
    }

    // 3. Begin listening
    if (!socket_listen(server_fd, 10)) {
        std::cerr << "[SERVER] listen() failed\n";
        return -1;
    }

    std::cout << "[SERVER] Listening on port " << SERVER_PORT << "...\n";

    // 4. Accept loop
    while (true) {
        SocketType client_fd = socket_accept(server_fd);
        if (client_fd < 0) {
            std::cerr << "[SERVER] accept() failed\n";
            continue;
        }

        std::cout << "[SERVER] Client connected!\n";

        // Make heap copy of fd for thread
        SocketType* fd_copy = new SocketType(client_fd);

        // Start client handling thread
        ThreadType t = thread_create(client_thread, fd_copy);
        thread_detach(t);
    }

    SocketClose(server_fd);
    socket_cleanup();
    return 0;
}
