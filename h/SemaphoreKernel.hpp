#ifndef PROJECT_BASE_V1_1_2_SEMAPHOREKERNEL_HPP
#define PROJECT_BASE_V1_1_2_SEMAPHOREKERNEL_HPP


#include "PCB.hpp"

typedef SemaphoreKernel* sem_t;

class SemaphoreKernel {

    int val;

public:
    List<PCB> blocked; // lista blokiranih
    SemaphoreKernel(int val_=1): val(val_){};

    static SemaphoreKernel* openSem(int init);
    void wait();
    void signal();
    int getVal(){return val;}

    static void* operator new (size_t size);
    static void operator delete (void* p);

};


#endif //PROJECT_BASE_V1_1_2_SEMAPHOREKERNEL_HPP
