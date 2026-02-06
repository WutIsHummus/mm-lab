#include "umalloc.h"
#include "csbrk.h"
#include <stdio.h>
#include <assert.h>
#include "ansicolors.h"

const char author[] = ANSI_BOLD ANSI_COLOR_RED "REPLACE WITH YOUR NAME AND UT EID" ANSI_RESET;

mem_block_header_t *free_heads[BIN_COUNT];
size_t bin_limits[BIN_COUNT - 1];

/*
 * The following helpers can be used to interact with the mem_block_header_t
 * struct, they can be adjusted as necessary.
 */

/*
 * block_metadata - returns true if a block is marked as allocated.
 */
bool is_allocated(mem_block_header_t *block) {
    assert(block != NULL);
    return block->block_metadata & 0x1;
}

/*
 * allocate - marks a block as allocated.
 */
void allocate(mem_block_header_t *block) {
    assert(block != NULL);
    block->block_metadata |= 0x1;
}

/*
 * deallocate - marks a block as unallocated.
 */
void deallocate(mem_block_header_t *block) {
    assert(block != NULL);
    block->block_metadata &= ~0x1;
}

/*
 * get_size - gets the size of the block.
 */
size_t get_size(mem_block_header_t *block) {
    assert(block != NULL);
    return block->block_metadata & ~(ALIGNMENT-1);
}

/*
 * get_next - gets the next block.
 */
mem_block_header_t *get_next(mem_block_header_t *block) {
    assert(block != NULL);
    return block->next;
}


/*
 * get_payload - gets the payload of the block.
 */
void *get_payload(mem_block_header_t *block) {
    assert(block != NULL);
    return (void*)(block + 1);
}

/*
 * get_header - given a payload, returns the block.
 */
mem_block_header_t *get_header(void *payload) {
    assert(payload != NULL);
    return ((mem_block_header_t *)payload) - 1;
}

/*
* The following are helper functions that can be implemented to assist in your
* design, but they are not required. 
*/

/*
 * set_block_metadata
 * Optional helper method that can be used to initialize the fields for the 
 * memory block struct. 
 */
void set_block_metadata(mem_block_header_t *block, size_t size, bool alloc) {
    // Optional Student TODO
}

/*
 * find - finds a free block that can satisfy the umalloc request.
 */
mem_block_header_t *find(size_t total_size) {
	// Optional Student TODO
	return NULL;
}

/*
 * extend - extends the heap if more memory is required.
 */
mem_block_header_t *extend(size_t size) {
    // Optional Student TODO
	return NULL;
}

/*
 * split - splits a given block in parts, one allocated, one free.
 */
mem_block_header_t *split(mem_block_header_t *block, size_t new_block_size) {
	// Optional Student TODO
	return NULL;
}

/*
* The following are functions that are required to be implemented for correctness. 
*/

/*
 * select_bin - selects a free list bin to use based on the 
 * block size. Returns an index
 * REQUIRED   
 */
size_t select_bin(size_t size) {
    // Student TODO
    return 0;
}

/**
 * set_bin_limits - initializes global bin_limits[] array
 * Set the limit as the max sized block you can allocate using a block
 * from that bin
 * REQUIRED
 */
void set_bin_limits() {
    // Student TODO
}

/**
 * coalesce - attempts to coalesce within a single bin
 * returns false if no coalesce is done at all, else return true
 * REQUIRED
 */
bool coalesce() {
    // Student TODO
	return NULL;
}


/*
 * uinit - Used initialize metadata required to manage the heap
 * along with allocating initial memory.
 * REQUIRED
 */
int uinit() {
    // Student TODO
    return 0;
}

/*
 * umalloc -  allocates size bytes and returns a pointer to the allocated memory.
 * REQUIRED
 */
void *umalloc(size_t size) {
	// Student TODO
    return NULL;
}

/**
 * ufree - frees the memory space pointed to by ptr.
 * ptr - pointer to payload of the memory to be freed, 
 * must have been called by a previous malloc call.
 * REQUIRED
 */
void ufree(void *ptr) {
    // Student TODO
}