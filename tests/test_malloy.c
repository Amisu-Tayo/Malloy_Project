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
    //void* ptr2 = malloy_alloc(1024 * 1024);  // 1 MB
   // void* ptr3 = malloy_alloc(1024 * 1024);  // 1 MB (should trigger expansion)

    printf("Allocated 3 blocks of 1MB each\n");

     // Free the allocated memory
    malloy_free(ptr1);
    //malloy_free(ptr2);
  //  malloy_free(ptr3);

    // Check memory status (should show updated total memory usage after expansion)
    malloy_status();
}    

int main() {
    // Calling Test functions
printf("Checking if pool is expanded when memory is over allocated");
test_pool_expansion();  // Test pool expansion

    return 0;
}