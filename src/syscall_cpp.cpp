#include "../h/syscall_cpp.hpp"


Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_no_start(&myHandle, body, arg);
}

Thread::~Thread() {
    delete myHandle;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    if(!myHandle)return -1;
    thread_start(myHandle);
    return 0;
}

Thread::Thread() {
    thread_create_no_start(&myHandle, func, this);
}

void Thread::func(void *arg) {
        Thread* t=(Thread*)arg;
        t->run();
}

void* operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
    return;
}

void* operator new[](size_t size){
    return MemoryAllocator::mem_alloc(size);
}

void operator delete[](void* ptr){
    MemoryAllocator::mem_free(ptr);
    return;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

void Console::putc(char c) {
    __putc(c);
}

char Console::getc() {
    return __getc();
}
