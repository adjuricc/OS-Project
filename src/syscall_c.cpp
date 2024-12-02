#include "../h/syscall_c.hpp"
void* mem_alloc(size_t size){
    return memAlloc(size);
}

int mem_free(void* allocated){
    return memFree(allocated);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    return threadCreate(handle, start_routine, arg);
}

int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg){
    return threadCreateNoStart(handle, start_routine, arg);
}

int thread_exit(){
    return threadExit();
}

void thread_dispatch(){
    threadDispatch();
}

int thread_start(thread_t handle){
    return threadStart(handle);
}

int sem_open(sem_t* handle, int init){
    return semOpen(handle, init);
}

int sem_close (sem_t handle){
    return semClose(handle);
}

int sem_wait (sem_t id){
    return semWait(id);
}

int sem_signal (sem_t id){
    return semSignal(id);
}

char getc(){
    return getC();
}

void putc(char c){
    putC(c);
}