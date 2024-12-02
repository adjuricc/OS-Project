#ifndef PROJECT_BASE_V1_1_2_PCB_HPP
#define PROJECT_BASE_V1_1_2_PCB_HPP


#include "Scheduler.hpp"

typedef PCB* thread_t;

class PCB
{
public:
    ~PCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    static void* operator new (size_t size);
    static void operator delete (void* p);


    using Body = void (*)(void*);

    static PCB *createThread(Body body, void* arg);
    static PCB *createThreadNoStart(Body body, void* arg);

    static int exitThread(){
        running->setFinished(true);
        dispatch();
        return 0;
    }

    static PCB *running;

private:
    PCB(Body body_, void* arg_, uint64 timeSlice_): body(body_), arg(arg_),
        stack(body!=nullptr ? (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64)) : nullptr),
        context({(uint64) &threadWrapper, stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
        finished(false)
        {
        if (body != nullptr) { Scheduler::put(this); }
        }

    PCB(Body body_, void* arg_, uint64 timeSlice_, bool dummy): body(body_), arg(arg_),
                                                    stack(body!=nullptr ? (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64)) : nullptr),
                                                    context({(uint64) &threadWrapper, stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
                                                    finished(false){}
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool finished;

    friend class Riscv;
    friend class SemaphoreKernel;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch(bool flagWhenBlocked=false, bool flagPutInScheduler=true);//flaguje se kad je u pitanju semafor

};


#endif //PROJECT_BASE_V1_1_2_PCB_HPP
