#ifndef PROJECT_BASE_V1_1_2_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_2_MEMORYALLOCATOR_HPP


#include "../lib/hw.h"

struct FreeSeg{
    FreeSeg* next, *prev;
    size_t size;//u blokovima
};


struct AllocatedSeg{
    size_t size;
};

class MemoryAllocator{

    static FreeSeg* head, *tail;
    static char* heap_start;
    static char* heap_end;

public:

    // inicijalizujemo vrednosti alokatora
    static void init(){
        heap_start=(char*)HEAP_START_ADDR;
        heap_end=(char*)HEAP_END_ADDR;
        size_t inBlocks=((char*)heap_end-(char*)heap_start + MEM_BLOCK_SIZE - 1) & ~(MEM_BLOCK_SIZE-1); // zaokuruzujemo heap na blokove
        head=(FreeSeg*)heap_start; tail=head;
        head->size=(size_t)inBlocks/MEM_BLOCK_SIZE; // upisujemo vrednost u blokovima
        head->next=nullptr; head->prev=nullptr;
    }

    static void* mem_alloc(size_t size){
        size+=sizeof(size_t);
        size=(size + MEM_BLOCK_SIZE - 1) & ~(MEM_BLOCK_SIZE-1);
        size_t blkNum=(size_t)size/MEM_BLOCK_SIZE;//zaokruzivanje
        FreeSeg* curr=head;

        while(curr){ // idemo po listi dok ne nadjemo prvi koji nam odgovara, tj. prvi koji ima dovoljno mesta
            if(curr->size>=blkNum)break;
            curr=curr->next;
        }

        if(!curr)return nullptr;

        size_t remaining_size=curr->size-blkNum; // za izracunavanje offseta, odakle pocinje novi blok
        if(curr->size>blkNum){ // slucaj kad nije iskoriscen ceo slobnodni fragment, vec ostaje deo
            curr->size-=blkNum; // smanjujemo velicinu fragmenta
            AllocatedSeg* newBlk = (AllocatedSeg*)((char*)curr -sizeof(FreeSeg)+ remaining_size*MEM_BLOCK_SIZE);// novi blok smestamo na kraj slobodnog fragmenta
            newBlk->size=blkNum;
            return (void*)((char*)newBlk-sizeof(FreeSeg)+sizeof(size_t));
        }
        else if(curr->size==blkNum){ // u slucaju da je jednak slobodnom fragmentu, onda moramo da taj sl frag izbacimo iz liste i prevezemo pokazivace
            if(curr->prev){
                curr->prev->next=curr->next;
            }
            else {
                head=curr->next;
            }

            if(curr->next) {
                curr->next->prev = curr->prev;
            }
            else {
                tail=curr->prev;
            }

            ((AllocatedSeg*)curr)->size = blkNum; // taj blok pretvaramo u alocirani blok

            return (void*)((char*)curr+sizeof(size_t)-sizeof(FreeSeg));
        }
        return nullptr;
    }

    static int mem_free(void* allocated){
        if((char*)allocated<heap_start || (char*)allocated>heap_end)return -1; // u slucaju da je van, greska

        AllocatedSeg* addr=(AllocatedSeg*)((char*)allocated-sizeof(size_t)); // posto upisujemo i velicinu moramo da se vratimo unazad da bismo i to dealocirali
        FreeSeg* next=head, *prev=nullptr;

        while(next){
            if((char*)next>(char*)addr) // dok ne nadjemo prvu vecu adresu
                break;
            prev=next;
            next=next->next;
        }
        if(next)prev=next->prev;

        // da li treba da se ubaci u listu slobodnih
        if(next && (char*)next>(char*)addr +addr->size*MEM_BLOCK_SIZE&& prev&& (char*)prev+prev->size*MEM_BLOCK_SIZE<(char*)addr){
            FreeSeg* newBlk=(FreeSeg*)addr;
            newBlk->next=next;
            newBlk->prev=prev;
            newBlk->size=addr->size;
            next->prev=newBlk;
            prev->next=newBlk;
        }
        else if(!prev && (char*)head>(char*)addr +addr->size*MEM_BLOCK_SIZE){ // ubacivanje u listu slobodnih u slucaju da je prvi u listi
            FreeSeg* newBlk=(FreeSeg*)addr;
            newBlk->next=next; newBlk->prev=nullptr;
            head=newBlk;
        }
        else if(!next && (char*)tail+tail->size*MEM_BLOCK_SIZE<(char*)addr){// ubacivanje u listu slobodnih u slucaju da je poslednji u listi
            FreeSeg* newBlk=(FreeSeg*)addr;
            newBlk->prev=tail; newBlk->next=nullptr;
            tail->next=newBlk;
            tail=newBlk;
        }
        else{ //ako treba da se merguje
            if((char*)next==(char*)addr+addr->size*MEM_BLOCK_SIZE){ // u slucaju da treba merge sa sledecim
                FreeSeg* newBlk=(FreeSeg*)addr;
                newBlk->next=next->next;
                newBlk->prev=prev;
                newBlk->size=(size_t)(addr->size+next->size);
                next=newBlk;
                if(!prev)head=newBlk;
            }
            if((char*)prev+prev->size*MEM_BLOCK_SIZE-sizeof(FreeSeg)==(char*)addr) { // u slucaju da treba merge sa prethodnim
                prev->size += addr->size;
            }

            if((char*)next==(char*)addr +addr->size*MEM_BLOCK_SIZE&& (char*)prev+prev->size*MEM_BLOCK_SIZE==(char*)addr){ // u slucaju da treba merge sa oba, onda moramo da prevezemo
                prev->next=next->next;
            }

        }
        return 0;
    }

};

#endif //PROJECT_BASE_V1_1_2_MEMORYALLOCATOR_HPP
