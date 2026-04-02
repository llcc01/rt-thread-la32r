#include <stddef.h>

#include "cache.h"

inline void dcache_invalid_writeback()
{
    for (size_t way = 0; way < DCACHE_WAY; way++)
    {
        for (size_t index = 0; index < (1 << DCACHE_INDEX_WIDTH); index++)
        {
            asm volatile("cacop 9, %0, 0"::"r"((index << DCACHE_OFFSET_WIDTH) | way));
        }
    }
}
