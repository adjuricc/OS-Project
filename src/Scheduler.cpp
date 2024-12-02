#include "../h/Scheduler.hpp"

List<PCB> Scheduler::readyCoroutineQueue;

PCB *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(PCB *pcb)
{
    readyCoroutineQueue.addLast(pcb);
}

void *Scheduler::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void Scheduler::operator delete(void *p) {
    MemoryAllocator::mem_free(p);
    return;
}
