#include <stdio.h>
#include "lazy_malloc.h"

// Declare the init_regions function (you may later expose it via API)



int main() {
    printf("Initializing LAMalloc regions...\n");
    init_regions();

    void* a = lamalloc(32);
    void* b = lamalloc(100);
    void* c = lamalloc(2048);

    printf("Short-lived alloc:  %p\n", a);
    printf("Medium-lived alloc: %p\n", b);
    printf("Long-lived alloc:   %p\n", c);


    return 0;
}