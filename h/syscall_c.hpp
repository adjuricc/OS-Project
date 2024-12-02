

#ifndef PROJECT_BASE_V1_1_2_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_2_SYSCALL_C_HPP

#include "PCB.hpp"
#include "SemaphoreKernel.hpp"
#include "../lib/console.h"
#include "abi_c.hpp"

void* mem_alloc(size_t size);

int mem_free(void* allocated);

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

int thread_start(thread_t handle);

int sem_open(sem_t* handle, int init);

int sem_close (sem_t handle);

int sem_wait (sem_t handle);

int sem_signal (sem_t id);

char getc();

void putc(char c);



#endif //PROJECT_BASE_V1_1_2_SYSCALL_C_HPP
