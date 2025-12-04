#include "../../common/include/socket_handler.hpp"
#include "../../common/include/thread_handler.hpp"
#include "../include/client_handler.hpp"

static const int SERVER_PORT = 5000;

int main() {
    std::cout << "[SERVER] Starting server..." << std::endl;

    SocketType server_fd = socket_create();
    if (server_fd < 0) {
        std::cerr << "[SERVER] socket_create() failed" << std::endl;
        return -1;
    }

    if (!socket_bind(server_fd, SERVER_PORT)) {
        std::cerr << "[SERVER] socket_bind() failed" << std::endl;
        return -1;
    }

    if (!socket_listen(server_fd)) {
        std::cerr << "[SERVER] socket_listen() failed" << std::endl;
        return -1;
    }

    std::cout << "[SERVER] Listening on port " << SERVER_PORT << "..." << std::endl;

    while (true) {
        SocketType client_fd = socket_accept(server_fd);
        if (client_fd < 0) {
            std::cerr << "[SERVER] socket_accept() failed" << std::endl;
            continue;
        }

        std::cout << "[SERVER] Client connected!" << std::endl;
        SocketType* fd_copy = new SocketType(client_fd);
        ThreadType t = thread_create(client_thread, fd_copy);
        thread_detach(t);
    }

    socket_close(server_fd);
    return 0;
}