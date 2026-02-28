#pragma once

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

struct HeapEntry
{
    size_t *ref_pos = NULL; // 记录下标，用于快速定位到堆中的位置
    uint64_t val = 0;       // 值

    const bool operator<(const HeapEntry &other) const
    {
        return this->val < other.val;
    }
    const bool less(const HeapEntry &other) const
    {
        return operator<(other);
    }
};

void heap_fix(HeapEntry *h, size_t len, size_t pos);