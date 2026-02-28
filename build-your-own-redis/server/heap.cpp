#include "heap.h"

#define HEAP_PARENT(i) (((i) - 1) >> 1)
#define HEAP_CHILD_LEFT(i) ((i) << 1 | 1)
#define HEAP_CHILD_RIGHT(i) ((i) << 1 | 2)

void heap_up(HeapEntry *h, size_t pos)
{
    // 采用挖坑填补法
    HeapEntry t = h[pos]; // 挖坑, 保留当前值

    for (; pos > 0;)
    {
        size_t i = HEAP_PARENT(pos);
        if (i == pos || !(t.less(h[i])))
        {
            break;
        }
        // 上浮
        h[pos] = h[i];           // swap with the parent
        *(h[pos].ref_pos) = pos; // record the position
        // 更新当前位置的索引
        pos = i;
    }
    h[pos] = t;              // 填补, 将当前值填补到坑中
    *(h[pos].ref_pos) = pos; // record the position
}

bool heap_down(HeapEntry *h, size_t pos, size_t len)
{
    // 采用挖坑填补法
    HeapEntry t = h[pos];
    size_t pos_old = pos;

    for (;;)
    {
        size_t l = HEAP_CHILD_LEFT(pos);
        if (l >= len)
        {
            break;
        }
        size_t r = HEAP_CHILD_RIGHT(pos);
        size_t j = (r < len && h[r].less(h[l])) ? r : l;
        if (!(h[j].less(t)))
        {
            break;
        }

        h[pos] = h[j];           // swap with the kid
        *(h[pos].ref_pos) = pos; // record the position
        // 更新当前位置的索引
        pos = j;
    }
    h[pos] = t;              // 填补, 将当前值填补到坑中
    *(h[pos].ref_pos) = pos; // record the position
    return pos > pos_old;
}

void heap_fix(HeapEntry *h, size_t len, size_t pos)
{
    if (!heap_down(h, pos, len))
    {
        heap_up(h, pos);
    }
}
