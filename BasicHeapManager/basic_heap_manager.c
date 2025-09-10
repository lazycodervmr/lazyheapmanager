
#include <basic_heap_manager.h>
size_t size_present_in_heap = 0; // Total size of the heap
void *head = NULL; // Pointer to the start of the heap

int allocate_space(size_t size, void **ptr_to_head){
    // Check if the size is valid
    if(size <= 0){
        return -1; // Invalid size
    }
    // Calculate the number of pages needed
    int num_of_pages = (size + sizeof(meta_data_t) ) / PAGE_SIZE;
    num_of_pages++ ; //rounding up to atleast 1 page.
    if(size_present_in_heap > size){
        
    }

}