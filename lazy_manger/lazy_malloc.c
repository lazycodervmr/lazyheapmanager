#include "lazy_malloc.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

Region regions[CLASSES_SPAN] = {0};

#define CALL_SITE_TABLE_SIZE 1024

typedef struct {
    uintptr_t call_site_hash;
    LazyMemoryType predicted_type;
} CallSiteEntry;

static CallSiteEntry call_site_table[CALL_SITE_TABLE_SIZE];

// --- Helpers ---
static inline uintptr_t get_call_site() {
    return (uintptr_t)__builtin_return_address(0);
}

static inline size_t hash_call_site(uintptr_t cs) {
    return (cs >> 4) % CALL_SITE_TABLE_SIZE;
}

// STEP 1: INIT REGIONS
void init_regions() {
    for(int i = 0; i < CLASSES_SPAN; i++ ){
        regions[i].type = (LazyMemoryType) i;
        regions[i].base = mmap(NULL, REGION_SIZE, PROT_READ | PROT_WRITE,  MAP_PRIVATE | MAP_ANON, -1, 0);
        if (regions[i].base == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }
        printf("Region %d base = %p\n", i, regions[i].base);
        regions[i].used = 0;
        memset(regions[i].bins, 0, sizeof(regions[i].bins));
    }
}

// STEP 2: LIFETIME PREDICTOR
static LazyMemoryType predict_lifetime(LazyMemoryType user_hint) {
    if (user_hint != HINT_NONE) {
        return user_hint;
    }
    uintptr_t cs = get_call_site();
    size_t idx = hash_call_site(cs);

    if (call_site_table[idx].call_site_hash == cs) {
        return call_site_table[idx].predicted_type;
    } else {
        LazyMemoryType default_type = MEDIUM_SPAN;
        call_site_table[idx].call_site_hash = cs;
        call_site_table[idx].predicted_type = default_type;
        return default_type;
    }
}

// STEP 3: LAZY MALLOC
void* lamalloc(size_t size, LazyMemoryType user_hint) {
    if (size == 0) return NULL;

    size = ALIGN(size);
    LazyMemoryType Mtype = predict_lifetime(user_hint);
    Region* region = &regions[Mtype];

    if (region->used + size > REGION_SIZE) {
        fprintf(stderr, "Region full, Allocation failed.\n");
        return NULL;
    }

    void* ptr = (char*)region->base + region->used;
    region->used += size;

    printf("[LAMalloc] size=%zu, class=%d, ptr=%p\n", size, Mtype, ptr);
    return ptr;
}
