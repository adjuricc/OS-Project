#include "../h/SemaphoreKernel.hpp"

void SemaphoreKernel::wait() {
    if(--val<0){
        blocked.addLast(PCB::running);
        PCB::dispatch(true);
    }
}

void SemaphoreKernel::signal(){
    if(++val<=0){
        PCB* pcb=blocked.removeFirst();
        Scheduler::put(pcb);
    }
}

SemaphoreKernel* SemaphoreKernel::openSem(int init) {
    return new SemaphoreKernel(init);
}

void *SemaphoreKernel::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void SemaphoreKernel::operator delete(void *p) {
    MemoryAllocator::mem_free(p);
    return;
}
