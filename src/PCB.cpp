#include "../h/PCB.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.hpp"

PCB *PCB::running = nullptr;


PCB *PCB::createThread(Body body, void* arg)
{
    PCB* pcb = new PCB(body, arg, DEFAULT_TIME_SLICE);
    return pcb;
}


void PCB::dispatch(bool flagWhenBlocked, bool flagPutInScheduler)
{
    PCB *old = running;
    if(!flagWhenBlocked && flagPutInScheduler) {
        if (!old->isFinished()) { Scheduler::put(old); }
    }
    running = Scheduler::get();
    PCB::contextSwitch(&old->context, &running->context);
}

void PCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    __asm__ volatile ("addi a0, zero, 0x13");
    __asm__ volatile ("ecall");
}

void *PCB::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void PCB::operator delete(void *p) {
    MemoryAllocator::mem_free(p);
    return;
}

PCB *PCB::createThreadNoStart(PCB::Body body, void *arg) {
    PCB* pcb = new PCB(body, arg, DEFAULT_TIME_SLICE, true);
    return pcb;
}

