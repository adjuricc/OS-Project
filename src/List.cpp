#include "../h/List.hpp"

template<typename T>
void *List<T>::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

template<typename T>
void List<T>::operator delete(void *p) {
    MemoryAllocator::mem_free(p);
    return;
}