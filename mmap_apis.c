#include <stdio.h>
#include <memory.h>
#include <unistd.h>   /*To get appropriate page size*/
#include <sys/mman.h> /*For using mmap*/

static size_t PAGE_SIZE = 0;

void mmap_init(){
    PAGE_SIZE = getpagesize();
}

/*Our own api to request kernel to allocate requested number of pages*/
static void * 
allocate_new_pages(int num_of_pages){
    char* start_ptr = mmap(
    0,                           // Let the kernel choose the memory address
    num_of_pages * PAGE_SIZE,    // The size of the mapping in bytes
    PROT_READ | PROT_WRITE | PROT_EXEC,  // Read, write, and execute permissions
    MAP_ANON | MAP_PRIVATE,      // Anonymous mapping, private to the process
    0, 0                         // No file descriptor, offset 0
);
    if(start_ptr == NULL){
        printf("ERROR: Allocation of memory failed");
        return NULL;
    }
    return (void *)start_ptr;
}

/*Our own api to request kernel to de-allocate requested number of pages*/
static void
deallocate_new_pages(void *start_ptr, int num_of_pages){
     if(munmap(start_ptr,num_of_pages*PAGE_SIZE)){
         printf("ERROR: De-allocation of memory failed");
     }
}

int main(int argc, char **argv){

    mmap_init();
    printf("The PAZE_SIZE is %lu\n", PAGE_SIZE);
    void* addr1 = allocate_new_pages(1);
    return 0;
}
