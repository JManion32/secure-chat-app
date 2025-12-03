#pragma once

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE ThreadType;
#else
    #include <pthread.h>
    typedef pthread_t ThreadType;
#endif

// Same signature across OSes
typedef void* (*ThreadFunc)(void*);

ThreadType thread_create(ThreadFunc func, void* arg);
void thread_detach(ThreadType t);
