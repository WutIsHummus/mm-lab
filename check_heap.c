#include "check_heap.h"

extern mem_block_header_t *free_head;
extern mem_block_header_t *free_heads[BIN_COUNT];

int check_heap() {
    for (int i = 0; i < BIN_COUNT; ++i) {
        int result = check_bin(free_heads[i]);
        if (result) return result;
    }
    return HEAP_SUCCESS;
}

/*
 * STUDENT TODO: set these variables according to your heap design (tests will fail if you do not
 * set these variables!)
 *    - order:      how is your free list ordered?
 *    - circular:   true if your free list is circular; false otherwise
 */
heap_order order = ORD_SIZE;
bool circular = false;


/*
 * get_bin_size - used to count the number of free blocks in a bin
 *
 * STUDENT TODO: this function is required to be completed for checkpoint 1
 *       - Ensure that each free block in the current bin is counted
 * 
 * Should return the number of free blocks in the current bin.
*/
int get_bin_size(size_t bin_idx) {   
    // Student TODO
    return 0;
}

/*
 * check_bin -  used to check that the heap is still in a consistent state.
 * 
 * STUDENT TODO: this function is required to be completed for checkpoint 1
 * 
 *      - Ensure that the free block list is in the order you expect it to be in
 *        (if your list is randomly ordered, this check is not required).
 * 
 *      - Check if any free blocks overlap with each other. 
 * 
 *      - Ensure that each free block is aligned.
 * 
 *      - Ensure that all blocks on the free list are free (no implicit free lists)
 *
 * Should return HEAP_SUCCESS if the heap is consistent or HEAP_FAILURE if an error 
 * is detected.
 */
int check_bin(mem_block_header_t *free_head) {
    // Student TODO
    return HEAP_SUCCESS;
}