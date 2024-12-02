#include "../h/MemoryAllocator.hpp"

char* MemoryAllocator::heap_start=nullptr;
char* MemoryAllocator::heap_end=nullptr;
FreeSeg* MemoryAllocator::head=nullptr;
FreeSeg* MemoryAllocator::tail=nullptr;
