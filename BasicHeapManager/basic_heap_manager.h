#ifndef BASIC_HEAP_MANAGER_H
#define BASIC_HEAP_MANAGER_H

#include <unistd.h>   /*To get appropriate page size*/
#include <memory.h>   /*To use mmap and munmap*/

#define PAGE_SIZE getpagesize() // Define PAGE_SIZE to the system's page size

// Structure to hold meta information of the datablock
typedef struct meta_data {
    size_t size_of_the_block; // Size of the block
    int free; // 1 if free, 0 if allocated
    struct meta_data *next; // Pointer to the next block
    void *start_address; // Pointer to the start of the block
} meta_data_t;

typedef struct heap_manager {

} heap_manager_t;


#endif BASIC_HEAP_MANAGER_H