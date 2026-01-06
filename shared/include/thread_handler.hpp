#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE ThreadType;
#else
    #include <pthread.h>
    typedef pthread_t ThreadType;
#endif

typedef void* (*ThreadFunc)(void*);

ThreadType thread_create(ThreadFunc func, void* arg);
void thread_detach(ThreadType t);
