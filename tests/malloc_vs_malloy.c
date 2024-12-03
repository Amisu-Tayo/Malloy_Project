#include "malloy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ALLOC_COUNT 10
#define ALLOC_SIZE 6

void test_malloc() {
    clock_t start = clock();
    for (int i = 0; i < ALLOC_COUNT; i++) {
        void* ptr = malloc(ALLOC_SIZE);
        free(ptr);
    }
    clock_t end = clock();
    printf("malloc time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("\n");
    printf("\n");
    printf("\n");
}

void test_malloy() {
    init_malloy();
    clock_t start = clock();
    for (int i = 0; i < ALLOC_COUNT; i++) {
        void* ptr = malloy_alloc(ALLOC_SIZE);
        malloy_free(ptr);
    }
    clock_t end = clock();
    printf("malloy time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Comparing malloc and malloy:\n");
    test_malloc();
    test_malloy();
    return 0;
}
