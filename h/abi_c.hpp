//
// Created by os on 2/16/23.
//

#ifndef PROJECT_BASE_V1_1_2_ABI_C_HPP
#define PROJECT_BASE_V1_1_2_ABI_C_HPP

#include "PCB.hpp"
#include "SemaphoreKernel.hpp"
#include "../lib/console.h"

void* memAlloc(size_t size);

int memFree(void* allocated);

int threadCreate(thread_t* handle, void(*start_routine)(void*), void* arg);

int threadCreateNoStart(thread_t* handle, void(*start_routine)(void*), void* arg);

int threadExit();

void threadDispatch();

int threadStart(thread_t handle);

int semOpen(sem_t* handle, int init);

int semClose (sem_t handle);

int semWait (sem_t handle);

int semSignal (sem_t id);

char getC();

void putC(char c);

#endif //PROJECT_BASE_V1_1_2_ABI_C_HPP
