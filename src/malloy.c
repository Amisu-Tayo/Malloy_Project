#include "malloy.h"         //  Header file for declarations 
#include <stdio.h>          //  For debugging/logging  
#include <stdlib.h>         //  For system malloc() 

#define MEMORY_POOL_SIZE 1024 *1024        // 1mb memory pool
static char memoryPool[MEMORY_POOL_SIZE]; // Simulated memory pool
static MemoryBlock* freeList = NULL;

//Initialize memory pool function definition
void init_malloy(){
    freeList = (MemoryBlock*) memoryPool;
    freeList->size = MEMORY_POOL_SIZE - sizeof(MemoryBlock);
    freeList->free = 1;
    freeList->next = NULL;
}

//Memory allocation function definitiuon
void* malloy_alloc(size_t size){
    if (!freeList) init_malloy();  //initialize malloy if not done

    MemoryBlock* current = freeList;
    while (current) {
        //look for a free memory block that can fit the requested size
        if (current->free && current->size >= size){
            current->free = 0; // mark tghe block as allocated 
            return (void*) (current + 1); //return memory after metadata
        }
        current = current->next;
    }
    return NULL; //No suitable block found
}

// Memory deallocation function definition
void malloy_free(void* ptr){
    if (!ptr) return;
    MemoryBlock* block = (MemoryBlock*) ptr -1; //Get metadata
    block-> free = 1; //Mark as free 

}
