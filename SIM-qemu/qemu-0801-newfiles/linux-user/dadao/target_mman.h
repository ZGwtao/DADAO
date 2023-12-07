/*
 * TASK_UNMAPPED_BASE         PAGE_ALIGN(TASK_SIZE / 3)
 */
#define TASK_UNMAPPED_BASE \
    TARGET_PAGE_ALIGN((1ull << (TARGET_VIRT_ADDR_SPACE_BITS - 1)) / 3)

#define ELF_ET_DYN_BASE       (TASK_UNMAPPED_BASE * 2)

#include "../generic/target_mman.h"
