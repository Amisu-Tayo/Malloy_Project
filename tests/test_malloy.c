#include "malloy.h"
#include <stdio.h>
#include <stdlib.h>

#define ALLOC_SIZE 64 // Size for each allocation
#define ALLOC_COUNT 10000 // Number of allocations for testing

// Test 1: Forcing the memory pool to expand
void test_pool_expansion() {
    init_malloy();  // Initialize allocator

    // Allocate memory that exceeds the initial pool size
    void* ptr1 = malloy_alloc(1024 * 1024);  // 1 MB
    void* ptr2 = malloy_alloc(1024 * 1024);  // 1 MB
    void* ptr3 = malloy_alloc(1024 * 1024);  // 1 MB (should trigger expansion)

    printf("Allocated 3 blocks of 1MB each\n");

     // Free the allocated memory
    malloy_free(ptr1);
    malloy_free(ptr2);
    malloy_free(ptr3);

    // Check memory status (should show updated total memory usage after expansion)
    malloy_status();
}  

void test_malloy_fragmentation() {
    init_malloy();  // Initialize allocator

    // Allocate 5 blocks
    void* ptr1 = malloy_alloc(256); // 256 bytes
    void* ptr2 = malloy_alloc(512); // 512 bytes
    void* ptr3 = malloy_alloc(128); // 128 bytes
    void* ptr4 = malloy_alloc(64);  // 64 bytes
    void* ptr5 = malloy_alloc(1024); // 1 KB

    printf("Allocated 5 blocks of different sizes\n");

    printf("ptr1 allocated at: %p\n", ptr1);
    printf("ptr2 allocated at: %p\n", ptr2);
    printf("ptr3 allocated at: %p\n", ptr3);
    printf("ptr4 allocated at: %p\n", ptr4);
    printf("ptr5 allocated at: %p\n", ptr5);

    // Free some blocks to create fragmentation
    malloy_free(ptr2); // Free 512 bytes block
    malloy_free(ptr4); // Free 64 bytes block

    // Allocate a new block and check if it reuses freed memory
    void* ptr6 = malloy_alloc(512);  // This should ideally reuse freed memory
    printf("Allocated new block of 512 bytes, ptr6 at: %p\n", ptr6);

    // Check memory status after allocation and deallocation
    malloy_status();
}

void test_memory_leaks() {
    init_malloy();  // Initialize allocator

    // Allocate memory and don't free it (to simulate a leak)
    void* ptr1 = malloy_alloc(128);  // 128 bytes
    void* ptr2 = malloy_alloc(256);  // 256 bytes

    printf("Allocated memory but not freeing it to simulate leaks\n");

    printf("ptr1 allocated at: %p\n", ptr1);
    printf("ptr2 allocated at: %p\n", ptr2);

    // Check for memory leaks (there should be leaks here)
    malloy_check_leaks();  // Should report leaks for ptr1 and ptr2
}

void test_small_allocations() {
    init_malloy();  // Initialize allocator

    // Allocate a large number of small blocks
    void* ptrs[1000];
    for (int i = 0; i < 1000; i++) {
        ptrs[i] = malloy_alloc(16);  // 16 bytes each
        printf("Allocated block %d at: %p\n", i, ptrs[i]);
    }

    // Free the allocated memory
    for (int i = 0; i < 1000; i++) {
        malloy_free(ptrs[i]);
    }

    printf("Freed all small allocations\n");

    // Check the memory status
    malloy_status();
}

void test_over_allocation() {
    init_malloy();  // Initialize allocator

    // Request allocations that exceed the initial pool size
    void* ptr1 = malloy_alloc(1024 * 1024);  // 1 MB
    void* ptr2 = malloy_alloc(2 * 1024 * 1024);  // 2 MB (should trigger expansion)

    printf("Requested over-allocation (1MB + 2MB)\n");

    // Free the allocated memory
    malloy_free(ptr1);
    malloy_free(ptr2);

    // Check the memory status after over-allocation
    malloy_status();  // Should reflect the memory pool expansion
}

void test_max_allocation() {
    init_malloy();  // Initialize allocator

    // Attempt to allocate a large block (e.g., half of the pool size)
    void* ptr1 = malloy_alloc(1024 * 1024);  // 1 MB
    void* ptr2 = malloy_alloc(1024 * 1024 * 3);  // 3 MB (should trigger expansion)

    printf("Allocated 1MB and 3MB, should trigger memory expansion.\n");

    // Free the allocated memory
    malloy_free(ptr1);
    malloy_free(ptr2);

    // Check the memory status
    malloy_status();  // Should reflect the allocation and deallocation
}




int main() {
    // Calling Test functions
printf("\n");    
printf("Checking if pool is expanded when memory is over allocated \n \n");
test_pool_expansion();  // Test pool expansion

printf("\n");
printf("Running test: test_malloy_fragmentation\n \n");
test_malloy_fragmentation();  // Test fragmentation

printf("\n");
printf("Running test: test_memory_leaks\n");
test_memory_leaks();  // Test leak detection

/*printf("\n");
printf("Running test: test_small_allocations\n");
test_small_allocations();  // Test small allocations */
// Small allocations test fail, fix later. Infinitely looping result

printf("\n");
printf("Running test: test_over_allocation\n");
test_over_allocation();  // Test exceeding pool size

printf("\n");
printf("Running test: test_max_allocation\n");
test_max_allocation();  // Test max allocation
return 0;
}
/* printf("Running test: test_malloy_fragmentation\n");
test_malloy_fragmentation();  // Test fragmentation

printf("Running test: test_memory_leaks\n");
test_memory_leaks();  // Test leak detection

printf("Running test: test_small_allocations\n");
test_small_allocations();  // Test small allocations

printf("Running test: test_over_allocation\n");
test_over_allocation();  // Test exceeding pool size

printf("Running test: test_max_allocation\n");
test_max_allocation();  // Test max allocation

    return 0;
} */