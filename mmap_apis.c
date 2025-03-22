#include <stdio.h>
#include <memory.h>
#include <unistd.h>   /*To get appropriate page size*/
#include <sys/mman.h> /*For using mmap*/

static size_t PAZE_SIZE = 0;

void mmap_init(){
    PAZE_SIZE = getpagesize();
}

/*Our own api to request kernel to allocate requested number of pages*/
static void * 
allocate_new_pages(int num_of_pages){
    char* start_ptr = mmap(
			   0,
                           num_of_pages*PAZE_SIZE,
			   PROT_READ|PROT_WRITE|PROT_EXEC,
                           MAP_ANON|MAP_PRIVATE,
                           0,0
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
     if(munmap(start_ptr,num_of_pages*PAZE_SIZE)){
         printf("ERROR: De-allocation of memory failed");
     }
}

int main(int argc, char **argv){

    mmap_init();
    printf("The PAZE_SIZE is %lu\n", PAZE_SIZE);
    void* addr1 = allocate_new_pages(1);
    return 0;
}
