#include <iostream>
#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"

static const int SERVER_PORT = 5000;

// Thread entry point for clients
void* client_thread(void* arg) {
    SocketType client_fd = *(SocketType*)arg;
    delete (SocketType*)arg;

    std::cout << "[SERVER] Client thread started" << std::endl;

    // Handle client here
    // e.g. read, write, etc.
    // socket_recv(client_fd, buffer, ...)

    socket_close(client_fd);
    return nullptr;
}

int main() {
    std::cout << "[SERVER] Starting server..." << std::endl;

    // 1. Create server socket
    SocketType server_fd = socket_create();
    if (server_fd < 0) {
        std::cerr << "[SERVER] socket_create() failed" << std::endl;
        return -1;
    }
    
    //std::cout << "File descriptor: " << server_fd << std::endl;

    // 2. Bind to port
    if (!socket_bind(server_fd, SERVER_PORT)) {
        std::cerr << "[SERVER] socket_bind() failed" << std::endl;
        return -1;
    }

    // 3. Begin listening
    if (!socket_listen(server_fd, 10)) {
        std::cerr << "[SERVER] socket_listen() failed" << std::endl;
        return -1;
    }

    std::cout << "[SERVER] Listening on port " << SERVER_PORT << "..." << std::endl;

    // 4. Accept loop
    while (true) {
        SocketType client_fd = socket_accept(server_fd);
        if (client_fd < 0) {
            std::cerr << "[SERVER] socket_accept() failed" << std::endl;
            continue;
        }

        std::cout << "[SERVER] Client connected!" << std::endl;

        // Make heap copy of fd for thread
        SocketType* fd_copy = new SocketType(client_fd);

        // Start client handling thread
        ThreadType t = thread_create(client_thread, fd_copy);
        thread_detach(t);
    }

    socket_close(server_fd);
    socket_cleanup();
    return 0;
}
