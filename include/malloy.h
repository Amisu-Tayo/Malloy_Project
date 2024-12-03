#ifndef malloy_h
#define malloy_h

#include <stddef.h> //for size_t

//metadata for memory blocks
typedef struct MemoryBlock {
    size_t size;                //size of memory block
    int free;                   //Free or allocated ( 0 or 1)
    struct MemoryBlock* next;   //pointer to next memory block
} MemoryBlock;

// Functions 
void* malloy_alloc(size_t size);
void malloy_free(void* ptr);
void init_malloy();

#endif