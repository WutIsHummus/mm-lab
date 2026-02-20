#include "check_heap.h"
#include <string.h>

extern mem_block_header_t *free_heads[BIN_COUNT];

static int addr_cmp(const void *a, const void *b) {
    mem_block_header_t *const *pa = a;
    mem_block_header_t *const *pb = b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return  1;
    return 0;
}

int check_heap() {
    /* Per-bin invariant checks */
    for (int i = 0; i < BIN_COUNT; ++i) {
        int result = check_bin(free_heads[i]);
        if (result) return result;
    }

    /* Cross-bin overlap check */
    int total = 0;
    for (int i = 0; i < BIN_COUNT; ++i)
        total += get_bin_size(i);

    if (total > 1) {
        mem_block_header_t *blocks[total];
        int idx = 0;
        for (int i = 0; i < BIN_COUNT; ++i)
            for (mem_block_header_t *c = free_heads[i]; c != NULL; c = c->next)
                blocks[idx++] = c;

        qsort(blocks, total, sizeof(blocks[0]), addr_cmp);

        for (int i = 0; i < total - 1; ++i) {
            char *end = (char *)blocks[i] + getSize(blocks[i]);
            if (end > (char *)blocks[i + 1])
                return HEAP_FAILURE;
        }
    }

    return HEAP_SUCCESS;
}

heap_order order = ORD_MEM;
bool circular = false;


int get_bin_size(size_t binIdx) {
    int count = 0;
    for (mem_block_header_t *curr = free_heads[binIdx]; curr != NULL; curr = curr->next)
        count++;
    return count;
}

/*
 * check_bin - validates consistency of a single free-list bin.
 *   - Address ordering
 *   - No overlapping blocks
 *   - All blocks aligned
 *   - All blocks marked free
 */

int check_bin(mem_block_header_t *freeHead) {
    mem_block_header_t *prev = NULL;
    for (mem_block_header_t *curr = freeHead; curr != NULL; curr = curr->next) {
        /* Alignment check */
        if ((uintptr_t)curr % ALIGNMENT != 0) return HEAP_FAILURE;

        /* Must be free */
        if (isAllocated(curr)) return HEAP_FAILURE;

        /* Address ordering */
        if (prev != NULL && prev >= curr) return HEAP_FAILURE;

        /* No overlap*/
        if (prev != NULL) {
            char *prevEnd = (char *)prev + getSize(prev);
            if (prevEnd > (char *)curr) return HEAP_FAILURE;
        }

        prev = curr;
    }
    return HEAP_SUCCESS;
}