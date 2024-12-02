
#include "../h/MemoryAllocator.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.hpp"

//extern void userMain();


int main(){
    MemoryAllocator::init();


    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);// postavlja da se koristi nasa prekidna rutina
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);// za prekide
    PCB::running= PCB::createThread(nullptr, nullptr);
//    thread_t userMainThread;
//    userMainThread=PCB::createThread(&userMainWrap, nullptr);
   // PCB::createThread(worker, nullptr);
    //thread_t userMainThread;
    //thread_create(&userMainThread, userMainWrap, nullptr);
    //userMainThread = nullptr;

    //Riscv::popSppSpie();
    //userMain();
    return 0;
}

