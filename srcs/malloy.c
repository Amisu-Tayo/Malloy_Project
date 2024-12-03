#include "malloy.h"
#include <stdio.h>          //  For debugging/logging  
#include <stdlib.h>         //  For system malloc() 
#include <time.h>           // For time comparison with malloc
#include <sys/mman.h>       // For mmap to dynamically allocate memory
#include <unistd.h>         // For sysconf
#include <string.h>         // for memset

#define CHUNK_SIZE (1024 *1024)        // 1 MB memory chinks to be added to heap
#define ALLOC_COUNT 10000              // Number of allocations for tests
#define ALLOC_SIZE 64                 // size of each allocation 



static MemoryPool* poolList = NULL; // List of memory pools
static size_t total_allocated = 0;  // Total allocated memory tracker

//Initialize first memory pool using mmap
void init_malloy() {
    MemoryPool* newPool = (MemoryPool*)mmap(NULL, sizeof(MemoryPool), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (newPool == MAP_FAILED) {
        perror("mmap failed for MemoryPool");
        exit(EXIT_FAILURE);
    }

    newPool->head = (MemoryBlock*)mmap(NULL, CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (newPool->head == MAP_FAILED) {
        perror("mmap failed for memory block");
        exit(EXIT_FAILURE);
    }

    newPool->head->size = CHUNK_SIZE - sizeof(MemoryBlock);
    newPool->head->free = 1;
    newPool->head->next = NULL;
    newPool->next = NULL;

    poolList = newPool;
    total_allocated = CHUNK_SIZE - sizeof(MemoryBlock);

}

// Expand memory pool dynamically using mmap
void expand_pool(){
    // Create a new/additional pool
    MemoryPool* newPool = (MemoryPool*)mmap(NULL, sizeof(MemoryPool), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,0);
     if (newPool == MAP_FAILED) {
        perror("mmap failed for MemoryPool");
        return;
    }

    newPool->head = (MemoryBlock*)mmap(NULL, CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (newPool->head == MAP_FAILED) {
        perror("mmap failed for new memory block");
        return;
    }

    newPool->head->size = CHUNK_SIZE - sizeof(MemoryBlock);
    newPool->head->free = 1;
    newPool->head->next = NULL;
    newPool->next = NULL;

    // Link the new pool to the existing pool list
    printf("Linking new pool at %p to the pool list\n", newPool);
    MemoryPool* current = poolList;
    while (current->next) {
        current = current->next;
    }
    current->next = newPool;

    total_allocated += CHUNK_SIZE - sizeof(MemoryBlock);
    printf("Memory pool expanded using mmap()\n");
}

//Memory allocation function to aloocate or expand memory dynamically if needed
void* malloy_alloc(size_t size){
    printf("Requesting allocation of size: %zu bytes\n", size);
    printf("Current total_allocated: %zu bytes\n", total_allocated);

    if (total_allocated + size > CHUNK_SIZE * 2) {  // Limit to 2 MB for test purposes
        printf("Warning: Memory usage exceeds threshold!\n");
    }

//Iterate over memory pools
    MemoryPool* pool = poolList;
    while (pool) {
        MemoryBlock* current = pool->head;
        while (current) {
            // Check if the block is free and large enough
            printf("Checking block at %p, size: %zu, free: %d\n", current, current->size, current->free);
            if (current->free && current->size >= size) {
                // If the block is larger than the requested size, split it
                if (current->size > size + sizeof(MemoryBlock)) {
                    // Create a new block for the remaining memory
                    MemoryBlock* newBlock = (MemoryBlock*)((char*)(current + 1) + size);
                    newBlock->size = current->size - size - sizeof(MemoryBlock);  // Size of the remaining block
                    newBlock->free = 1;  // Mark the new block as free
                    newBlock->next = current->next;  // Link it to the next block

                    // Resize the current block to the requested size
                    current->size = size;
                    current->next = newBlock;  // Link the current block to the new block
                }
                current->free = 0;  // Mark the block as allocated
                total_allocated += size; // Update total allocate dmemory
                printf("Allocated block at %p, size: %zu\n", (void*)(current + 1), current->size);
                return (void*)(current + 1);  // Return the memory 
            }
            current = current->next;
        }
        pool = pool->next;  // Move to the next pool
    }

    expand_pool(); // Expand the pool if no suitable block is found
    //return malloy_alloc(size);  // Retry allocation
    return 0;
    
}

// Free allocated memory and coalesce adjacent blocks
void malloy_free(void* ptr){
    if (!ptr) return;
    MemoryBlock* block = (MemoryBlock*) ptr -1; //Get metadata
    block-> free = 1; //Mark as free 
    total_allocated -= block->size; // Update allocated memory

    // Coalescing adjacent free blocks
    MemoryPool* pool = poolList;
    while (pool) {
        MemoryBlock* current = pool->head;
        while (current && current->next) {
            // check if both current and next blocks are free
            if (current->free && current->next->free) {
                current->size += sizeof(MemoryBlock) + current->next->size;  //merge the two blocks
                current->next = current->next->next;  // remove the next block from the list
            } else {
                current = current->next;   //if no merging occurs, move to the next block
            }
        }
        pool = pool->next;
    }

}
// Track the status of allocated and free memory
void malloy_status() {
    printf("Total allocated memory: %zu bytes\n", total_allocated);
    MemoryPool* pool = poolList;
    size_t free_memory = 0; //Track free memory

    //Traverse all memory pools to calculate total free memory
    while (pool) {
        MemoryBlock* current = pool->head;
        while (current) {
            if (current->free) {
                free_memory += current->size;
            }
            current = current->next;
        }
        pool = pool->next;
    }
    printf("Total free memory: %zu bytes\n", free_memory);
}

// Check for memory leaks
void malloy_check_leaks() {
    MemoryPool* pool = poolList;
    while (pool) {
        MemoryBlock* current = pool->head;
        while (current) {
            if (!current->free) {
                printf("Memory leak detected: Block of size %zu at %p not freed\n", current->size, (void*)(current + 1));
            }
            current = current->next;
        }
        pool = pool->next;
    }
}

// Fragmentation test - checks the reuse of free blocks
void test_fragmentation() {
    void* ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloy_alloc(ALLOC_SIZE);
        printf("Allocated %d: %p\n", i, ptrs[i]);
    }

    for (int i = 0; i < 10; i += 2) {
        malloy_free(ptrs[i]);
        printf("Freed %d: %p\n", i, ptrs[i]);
    }

    // Allocate again to test reuse of freed blocks
    for (int i = 0; i < 5; i++) {
        void* new_ptr = malloy_alloc(ALLOC_SIZE);
        printf("Re-allocated: %p\n", new_ptr);
    }
}

// Time test for malloc and malloy
void time_test() {
    clock_t start, end;
    start = clock();

    for (int i = 0; i < ALLOC_COUNT; i++) {
        void* ptr = malloc(ALLOC_SIZE);
        free(ptr);
    }

    end = clock();
    printf("malloc time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();

    for (int i = 0; i < ALLOC_COUNT; i++) {
        void* ptr = malloy_alloc(ALLOC_SIZE);
        malloy_free(ptr);
    }

    end = clock();
    printf("malloy time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

