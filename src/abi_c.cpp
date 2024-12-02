#include "../h/abi_c.hpp"

void* memAlloc(size_t size){
    __asm__ volatile("mv a1, %0"::"r"(size));//upisujemo argument u a1, size u bajtovima
    __asm__ volatile("li a0, 0x01"); //upisujemo vrednost prekidne rutine u a0
    __asm__ volatile("ecall");
    void* ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));// citamo vrednost reg a0 posto se preko njega vraca vr fje i upisujemo u ret
    return ret;
}

int memFree(void* allocated){
    __asm__ volatile("mv a1, %0"::"r"(allocated));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}

int threadCreate(thread_t* handle, void(*start_routine)(void*), void* arg){
    static thread_t* _handle; // zbog a0 overwriteovanja registra
    _handle = handle;
    __asm__ volatile("mv a7, %0"::"r"(arg));
    __asm__ volatile("mv a6, %0"::"r"(start_routine));
    __asm__ volatile("li a0, 0x11");

    __asm__ volatile("ecall");
    thread_t ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    *_handle=ret;
    if(!ret)return -1;
    return 0;
}

int threadCreateNoStart(thread_t* handle, void(*start_routine)(void*), void* arg){
    static thread_t* _handle; // zbog a0 overwriteovanja registra
    _handle = handle;
    __asm__ volatile("mv a7, %0"::"r"(arg));
    __asm__ volatile("mv a6, %0"::"r"(start_routine));
    __asm__ volatile("li a0, 0x15");

    __asm__ volatile("ecall");
    thread_t ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    *_handle=ret;
    if(!ret)return -1;
    return 0;
}

int threadExit(){
    __asm__ volatile("li a0, 0x12");

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

void threadDispatch(){
    __asm__ volatile("li a0, 0x13");

    __asm__ volatile("ecall");
}

int threadStart(thread_t handle){

    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x14");

    __asm__ volatile("ecall");

    return 0;
}

int semOpen(sem_t* handle, int init){
    static sem_t* _handle; // zbog a0 overwriteovanja registra
    _handle = handle;
    __asm__ volatile("mv a7, %0"::"r"(init));
    __asm__ volatile("li a0, 0x21");

    __asm__ volatile("ecall");

    sem_t ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    *_handle=ret;
    if(!ret)return -1;
    return 0;
}

int semClose (sem_t handle){
    __asm__ volatile("mv a6, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x22");

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int semWait (sem_t id){
    __asm__ volatile("mv a6, %0"::"r"(id));
    __asm__ volatile("li a0, 0x23");

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int semSignal (sem_t id){
    __asm__ volatile("mv a6, %0"::"r"(id));
    __asm__ volatile("li a0, 0x24");

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

char getC(){
    return __getc();
}

void putC(char c){
    __putc(c);
}
