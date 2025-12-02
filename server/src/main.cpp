#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

static const int SERVER_PORT = 5000;

// Thread wrapper to match pthread signature
void* client_thread(void* arg) {
    int client_fd = *(int*)arg;
    delete (int*)arg;  // allocated in main, free it here

    close(client_fd);
    return nullptr;
}

int main() {
    std::cout << "[SERVER] Starting server..." << std::endl;

    // 1. Create TCP socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("[SERVER] socket() failed");
        exit(EXIT_FAILURE);
    }

    // Allow immediate reuse of address after restart
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind socket to port
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // listen on all interfaces
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[SERVER] bind() failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 10) < 0) {
        perror("[SERVER] listen() failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "[SERVER] Listening on port " << SERVER_PORT << "..." << std::endl;

    // 4. Accept loop (multithreaded)
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("[SERVER] accept() failed");
            continue;
        }

        std::cout << "[SERVER] Client connected from "
                  << inet_ntoa(client_addr.sin_addr)
                  << ":" << ntohs(client_addr.sin_port)
                  << std::endl;

        // Allocate fd for thread safety
        int* thread_fd = new int(client_fd);

        pthread_t tid;
        if (pthread_create(&tid, nullptr, client_thread, thread_fd) != 0) {
            perror("[SERVER] pthread_create failed");
            close(client_fd);
            delete thread_fd;
            continue;
        }

        pthread_detach(tid);  // we don't join, thread cleans itself up
    }

    close(server_fd);
    return 0;
}
