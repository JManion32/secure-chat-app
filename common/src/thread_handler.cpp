#include "../include/thread_handler.hpp"

#ifdef _WIN32

// Windows thread wrapper
DWORD WINAPI thread_start(LPVOID param) {
    void** package = (void**)param;
    ThreadFunc func = (ThreadFunc)package[0];
    void* arg = package[1];
    delete[] package;

    func(arg);
    return 0;
}

ThreadType thread_create(ThreadFunc func, void* arg) {
    void** package = new void*[2]{ (void*)func, arg };
    return CreateThread(nullptr, 0, thread_start, package, 0, nullptr);
}

void thread_detach(ThreadType t) {
    CloseHandle(t);
}

#else

// Linux pthread wrapper
void* thread_start(void* param) {
    void** package = (void**)param;
    ThreadFunc func = (ThreadFunc)package[0];
    void* arg = package[1];
    delete[] package;

    return func(arg);
}

ThreadType thread_create(ThreadFunc func, void* arg) {
    ThreadType t;
    void** package = new void*[2]{ (void*)func, arg };
    pthread_create(&t, nullptr, thread_start, package);
    return t;
}

void thread_detach(ThreadType t) {
    pthread_detach(t);
}

#endif
