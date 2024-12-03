#ifndef malloy_h
#define malloy_h

#include <stddef.h> //for size_t

//memory block structure definition
typedef struct MemoryBlock {
    size_t size;                //size of memory block
    int free;                   //Free or allocated (1 if free, 0 if allocated)
    struct MemoryBlock* next;   //pointer to next memory block
} MemoryBlock;

//Memory Pool structure definition
typedef struct MemoryPool {
    MemoryBlock* head;          // Pointer to the first block in the memory pool
    struct MemoryPool* next;    // Pointer to next memory pool
} MemoryPool;

// Functions 
void init_malloy();
void* malloy_alloc(size_t size);
void malloy_free(void* ptr);
void cleanup_memory();
void malloy_status();
void malloy_check_leaks();
void test_fragmentation();
void time_test();

#endif