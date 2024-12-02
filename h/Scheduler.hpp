//
// Created by os on 2/8/23.
//

#ifndef PROJECT_BASE_V1_1_2_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_2_SCHEDULER_HPP


#include "List.hpp"

class PCB;
class SemaphoreKernel;

class Scheduler
{
private:
    static List<PCB> readyCoroutineQueue; // red spremnih niti

public:
    static PCB *get();

    static void put(PCB *ccb); // stavlja na kraj reda

    static void* operator new (size_t size);
    static void operator delete (void* p);
};

#endif //PROJECT_BASE_V1_1_2_SCHEDULER_HPP
