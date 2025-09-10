/*
lazy_malloc.h
author : V M Ranjith
Version: 1.0
Date   : 24/06/2025
*/

#ifndef LAZY_MALLOC_H
#define LAZY_MALLOC_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define REGION_SIZE   (64 * 1024)   // 64 KB per region
#define MAX_BINS      16            // Bin size classes
#define MIN_ALLOC     16
#define ALIGN(size)   (((size) + MIN_ALLOC - 1) & ~(MIN_ALLOC - 1))
#define MAX_CALL_SITES 1024

// --- Lifetime Classes ---
typedef enum {
    HINT_NONE = -1,
    SHORT_SPAN = 0,
    MEDIUM_SPAN,
    LARGE_SPAN,
    CLASSES_SPAN
} LazyMemoryType;

// --- Call-Site Table Entry ---
typedef struct {
    uintptr_t call_site_hash;
    LazyMemoryType predicted_type;
} CallSiteEntry;

// --- Chunk Metadata ---
typedef struct Chunk {
    size_t size;
    bool in_use;
    struct Chunk* next;
} Chunk;

// --- Region (Arena) ---
typedef struct Region {
    LazyMemoryType type;
    void* base;
    size_t used;
    Chunk* bins[MAX_BINS];
} Region;

// --- Public API ---
void init_regions(void);
void* lamalloc(size_t size, LazyMemoryType user_hint);
void lafree(void* ptr);

// --- Call-site heuristic helpers ---
static inline uintptr_t get_call_site(void) {
    return (uintptr_t)__builtin_return_address(0);
}

static inline size_t hash_call_site(uintptr_t addr) {
    return addr % MAX_CALL_SITES;
}

// --- Call-site table (defined in .c) ---
extern CallSiteEntry call_site_table[MAX_CALL_SITES];

#endif // LAZY_MALLOC_H
