#include "../h/Riscv.hpp"
#include "../lib/console.h"
#include "../h/SemaphoreKernel.hpp"


using Body = void (*)(void*);
void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 a0;
    __asm__ volatile("mv %[a0], a0":[a0]"=r"(a0));
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        if(a0==0x01){
            size_t size;
            __asm__ volatile("mv %[a1], a1":[a1]"=r"(size));
            void* ret=MemoryAllocator::mem_alloc(size);
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }
        else if(a0==0x02){
            void* allocated;
            __asm__ volatile("mv %[a1], a1":[a1]"=r"(allocated));
            int ret=MemoryAllocator::mem_free(allocated);
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }
        else if(a0==0x11){
            void* arg; Body body;
            __asm__ volatile("mv %[a7], a7":[a7]"=r"(arg));
            __asm__ volatile("mv %[a6], a6":[a6]"=r"(body));
            thread_t pcb=PCB::createThread(body, arg);
            __asm__ volatile("mv a0, %0"::"r"(pcb));
        }
        else if(a0==0x12){
            int ret=PCB::exitThread();
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }
        else if(a0 == 0x13){
            PCB::dispatch();
        }
        else if(a0==0x14){
            thread_t handle;
            __asm__ volatile("mv %[a1], a1":[a1]"=r"(handle));

            Scheduler::put(handle);
        }
        else if(a0==0x15){
            void* arg; Body body;
            __asm__ volatile("mv %[a7], a7":[a7]"=r"(arg));
            __asm__ volatile("mv %[a6], a6":[a6]"=r"(body));
            thread_t pcb=PCB::createThreadNoStart(body, arg);
            __asm__ volatile("mv a0, %0"::"r"(pcb));
        }
        else if(a0==0x21){
            int init;
            __asm__ volatile("mv %[a7], a7":[a7]"=r"(init));

            sem_t sem=SemaphoreKernel::openSem(init);
            __asm__ volatile("mv a0, %0"::"r"(sem));
        }
        else if(a0==0x22){
            sem_t handle;
            __asm__ volatile("mv %[a6], a6":[a6]"=r"(handle));

            while(handle->blocked.peekFirst()!=nullptr){
                PCB* pcb=(handle->blocked).removeFirst();
                Scheduler::put(pcb);
            }
            int ret=0;
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }
        else if(a0==0x23){
            sem_t handle;
            __asm__ volatile("mv %[a6], a6":[a6]"=r"(handle));
            int ret;
            if(!handle)ret=1;
            ret=0;
            handle->wait();
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }
        else if(a0==0x24){
            sem_t handle;
            __asm__ volatile("mv %[a6], a6":[a6]"=r"(handle));
            int ret;
            if(!handle)ret=1;
            ret=0;
            handle->signal();
            __asm__ volatile("mv a0, %0"::"r"(ret));
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        __putc('q');
    }
}