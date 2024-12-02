#ifndef PROJECT_BASE_V1_1_2_LIST_HPP
#define PROJECT_BASE_V1_1_2_LIST_HPP

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

template<typename T>

class List {

    struct Elem{
        T* data; Elem* next;

        Elem(T* data_, Elem* next_){
            data=data_; next=next_;
        }

        static void* operator new (size_t size){
            return MemoryAllocator::mem_alloc(size);
        }
        static void operator delete (void* p){
            MemoryAllocator::mem_free(p);
            return;
        }
    };
    Elem* head, *tail;

public:
    List():head(nullptr), tail(nullptr){}

    List(const List<T> &)=delete;

    List<T>& operator=(const List<T> &)=delete;

    static void* operator new (size_t size);
    static void operator delete (void* p);

    void addLast(T* data){
        Elem* elem=new Elem(data, 0);
        if(tail){
            tail->next=elem;
            tail=elem;
        }
        else head=tail=elem;
    }

    T* removeFirst(){
        if(!head)return 0;

        Elem* elem=head;
        head=head->next;
        if(!head)tail=0;

        T* ret=elem->data;
        delete elem;
        return ret;
    }

    T* peekFirst(){
        if(!head)return 0;
        return head->data;
    }

};


#endif //PROJECT_BASE_V1_1_2_LIST_HPP
