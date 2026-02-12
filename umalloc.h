#include <stdlib.h>
#include <stdbool.h>

#define ALIGNMENT 16 /* The alignment of all payloads returned by umalloc */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define BIN_COUNT 6

/*
 * mem_block_header_t - Represents a block of memory managed by the heap. The 
 * struct can be left as is, or modified for your design.
 * In the current design bit0 is the allocated bit
 * bits 1-3 are unused.
 * and the remaining 60 bit represent the size.
 * Caution: The size is a 64 bit number, not a 60 bit number
 * Because headers must be 16B aligned, the lower 4 bits of the size are irrelevant
 */
typedef struct mem_block_header_struct {
    size_t block_metadata; // This field stores the representation of the block size in bits [63:4],
                            // and allocation status in bit 0
    struct mem_block_header_struct *next;
    struct mem_block_header_struct *prev;
    size_t _padding; // Pad to 32 bytes so payload is 16-byte aligned
} mem_block_header_t;

// Helper Functions — camelCase, const-correct where pure.
bool isAllocated(const mem_block_header_t *block);
void allocate(mem_block_header_t *block);
void deallocate(mem_block_header_t *block);
size_t getSize(const mem_block_header_t *block);
mem_block_header_t *getNext(const mem_block_header_t *block);
void *getPayload(const mem_block_header_t *block);
mem_block_header_t *getHeader(void *payload);

void setBlockMetadata(mem_block_header_t *block, size_t size, bool alloc);
mem_block_header_t *find(size_t totalSize);
mem_block_header_t *extend(size_t size);
mem_block_header_t *split(mem_block_header_t *block, size_t newBlockSize);

// Portion that may not be edited
size_t select_bin(size_t size);
void set_bin_limits();
bool coalesce();
int uinit();
void *umalloc(size_t size);
void ufree(void *ptr);