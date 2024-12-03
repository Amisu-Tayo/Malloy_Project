#include "malloy.h"
#include <stdio.h>

int main(){
    //initialize the  of the memory pool
    init_malloy();

    //Demo allocations
    void* ptr1 = malloy_alloc(100);
    printf("Allocated 100 bytes at %p\n", ptr1);

    void* ptr2 = malloy_alloc(200);
    printf("Allocated 100 bytes at %p\n", ptr2);

    void* ptr3 = malloy_alloc(500);
    printf("Allocated 100 bytes at %p\n", ptr3);

    return 0;
}