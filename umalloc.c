#include "umalloc.h"
#include "csbrk.h"
#include <stdio.h>
#include <assert.h>
#include "ansicolors.h"

const char author[] = ANSI_BOLD ANSI_COLOR_RED "Alperen Aydin aa95287" ANSI_RESET;

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


static size_t calcSize(size_t size) {
    size_t total_size = size + sizeof(mem_block_header_t) ;
    return total_size + (ALIGNMENT - (total_size % ALIGNMENT)) % ALIGNMENT;
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


size_t getSize(const mem_block_header_t *block) {
    assert(block != NULL);
    return block->block_metadata & ~(ALIGNMENT - 1);
}

bool isAllocated(const mem_block_header_t *block) {
    assert(block != NULL);
    return block->block_metadata & 0x1;
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
	size_t bin = select_bin(total_size);

    for (int i = bin; i < BIN_COUNT; i++){
        mem_block_header_t *cur = free_heads[i];
        while (cur){
            if (get_size(cur) >= total_size){
                return cur;
            }
            cur = cur->next;
        }
    }
    return NULL;
}

/*
 * extend - extends the heap if more memory is required.
 */
mem_block_header_t *extend(size_t size) {
    size_t alloc_size = size > PAGESIZE ? size : PAGESIZE;
    alloc_size = ((alloc_size + PAGESIZE - 1) / PAGESIZE) * PAGESIZE;

    mem_block_header_t *newBlock = csbrk(alloc_size);
    if (newBlock == NULL){
        return NULL;
    }

    newBlock->block_metadata = alloc_size;
    newBlock->next = NULL;
    newBlock->prev = NULL;

    size_t bin = select_bin(alloc_size);

    mem_block_header_t* cur = free_heads[bin];
    mem_block_header_t* prev = NULL;

    while (cur != NULL && cur < newBlock){
        prev = cur;
        cur = cur->next;
    }


    newBlock->next = cur;
    newBlock->prev = prev;

    if (cur) {
        cur->prev = newBlock;
    }

    if (prev) {
        prev->next = newBlock;
    }
    else {
        free_heads[bin] = newBlock;
    }

    return newBlock;
}

/*
 * split - splits a given block in parts, one allocated, one free.
 */
mem_block_header_t *split(mem_block_header_t *block, size_t new_block_size) {
    size_t oldSize = get_size(block);
    size_t remainingSize = oldSize - new_block_size;

    block->block_metadata = new_block_size;

    mem_block_header_t* newBlock = (mem_block_header_t*) ((char*) block + new_block_size);

    newBlock->block_metadata = remainingSize;
    newBlock->next = NULL;
    newBlock->prev = NULL;

    size_t bin = select_bin(remainingSize);

    mem_block_header_t* cur = free_heads[bin];
    mem_block_header_t* prev = NULL;

    while (cur != NULL && newBlock < cur){
        prev = cur;
        cur = cur->next;
    }


    newBlock->next = cur;
    newBlock->prev = prev;

    if (cur) {
        cur->prev = newBlock;
    }

    if (prev) {
        prev->next = newBlock;
    }
    else {
        free_heads[bin] = newBlock;
    }

    return newBlock;
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
    for (size_t i = 0; i < BIN_COUNT - 1; ++i) {
        if (size <= bin_limits[i]) {
            return i;
        }
    }
    return BIN_COUNT - 1;
}

/**
 * set_bin_limits - initializes global bin_limits[] array
 * Set the limit as the max sized block you can allocate using a block
 * from that bin
 * REQUIRED
 */
void set_bin_limits() {
    bin_limits[0] = 64;
    bin_limits[1] = 128;
    bin_limits[2] = 256;
    bin_limits[3] = 512;
    bin_limits[4] = 1024;
}

/**
 * coalesce - attempts to coalesce within a single bin
 * returns false if no coalesce is done at all, else return true
 * REQUIRED
 */

static int addr_cmp(const void *a, const void *b) {
    mem_block_header_t *const *pa = a;
    mem_block_header_t *const *pb = b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return  1;
    return 0;
}

bool coalesce() {
    int total = 0;
    for (int i = 0; i < BIN_COUNT; ++i) {
        for (mem_block_header_t *c = free_heads[i]; c; c = c->next){
            total++;
        }
    }

    if (total < 2) return false; // sad non to merge

    mem_block_header_t *blocks[total];
    int idx = 0;
    for (int i = 0; i < BIN_COUNT; ++i){
        for (mem_block_header_t *c = free_heads[i]; c; c = c->next){
             blocks[idx++] = c;
        }
    }

    // built in sort wow
    qsort(blocks, total, sizeof(blocks[0]), addr_cmp);


    for (int i = 0; i < BIN_COUNT; ++i){
        free_heads[i] = NULL;
    }

    // merge adjacent
    bool coalesced = false;
    int i = 0;
    while (i < total) {
        mem_block_header_t *block = blocks[i];
        size_t size = get_size(block);

        int j = i + 1;

        while (j < total) {
            if ((char *)block + size == (char *)blocks[j]) {
                size += get_size(blocks[j]);
                coalesced = true;
                j++;
            } else {
                break;
            }
        }

        block->block_metadata = size;
        block->next = NULL;
        block->prev = NULL;

        size_t bin = select_bin(size);

        mem_block_header_t* cur = free_heads[bin];
        mem_block_header_t* prev = NULL;

        while (cur != NULL && cur < block){
            prev = cur;
            cur = cur->next;
        }

        block->next = cur;
        block->prev = prev;

        if (cur) {
            cur->prev = block;
        }

        if (prev) {
            prev->next = block;
        }
        else {
            free_heads[bin] = block;
        }

        i = j;
    }

    return coalesced;
}


/*
 * uinit - Used initialize metadata required to manage the heap
 * along with allocating initial memory.
 * REQUIRED
 */
int uinit() {
    for (int i = 0; i < BIN_COUNT; i++) {
        free_heads[i] = NULL;
    }
    
    set_bin_limits();


    mem_block_header_t* initial_block = csbrk(PAGESIZE);
    if (initial_block == NULL) {
        return -1;
    }


    initial_block->block_metadata = PAGESIZE;
    initial_block->next = NULL;
    initial_block->prev = NULL;

    size_t bin = select_bin(PAGESIZE);
    free_heads[bin] = initial_block;

    return 0;
}

/*
 * umalloc -  allocates size bytes and returns a pointer to the allocated memory.
 * REQUIRED
 */
void *umalloc(size_t size) {
	if (size == 0) {
        return NULL;
    }
    size_t totalSize = calcSize(size);
    mem_block_header_t* block = find(totalSize);

    if (block == NULL) { 
        if (coalesce()) {
            block = find(totalSize);
        }
        if (block == NULL) {
            block = extend(totalSize);
            if (block == NULL) {
                return NULL;
            }
        }
    }

    size_t bin = select_bin(get_size(block));

    if (block->prev != NULL) {
        block->prev->next = block->next;
    } else {
        free_heads[bin] = block->next; // first block of bin
    }

    if (block->next != NULL) {
        block->next->prev = block->prev;
    }

    size_t blockSize = get_size(block);
    size_t minSize = sizeof(mem_block_header_t); 
    if (blockSize >= totalSize + minSize) {
        split(block, totalSize);
    }
    allocate(block);
    return get_payload(block);
}


/**
 * ufree - frees the memory space pointed to by ptr.
 * ptr - pointer to payload of the memory to be freed, 
 * must have been called by a previous malloc call.
 * REQUIRED
 */
void ufree(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    mem_block_header_t *block = get_header(ptr);
    deallocate(block);

    size_t size = get_size(block);
    size_t bin = select_bin(size);

    mem_block_header_t* curr = free_heads[bin];
    mem_block_header_t* prev = NULL;

    while (curr != NULL && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    block->next = curr;
    block->prev = prev;
    if (prev != NULL) {
        prev->next = block;
    } else {
        free_heads[bin] = block; // first block of bin
    }
    
    if (curr != NULL) {
        curr->prev = block;
    }

}