#pragma once

#include <functional>
#include <stddef.h>

template <typename T> struct HeapEntry {
  size_t *ref_pos; // 记录下标，用于快速定位到堆中的位置
  T val;           // 值

  HeapEntry() : ref_pos(nullptr), val(T()) {}
  HeapEntry(size_t *ref_pos, T val) : ref_pos(ref_pos), val(val) {}
};

#define HEAP_PARENT(i) (((i) - 1) >> 1)
#define HEAP_CHILD_LEFT(i) ((i) << 1 | 1)
#define HEAP_CHILD_RIGHT(i) ((i) << 1 | 2)

template <typename T, typename Compare = std::less<T>>
void heap_up(HeapEntry<T> *h, size_t pos) {
  Compare __less;

  // 采用挖坑填补法
  HeapEntry<T> t = h[pos]; // 挖坑, 保留当前值

  for (; pos > 0;) {
    size_t i = HEAP_PARENT(pos);
    if (!(__less(t.val, h[i].val))) {
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

template <typename T, typename Compare = std::less<T>>
bool heap_down(HeapEntry<T> *h, size_t pos, size_t len) {
  Compare __less;
  // 采用挖坑填补法
  HeapEntry<T> t = h[pos];
  size_t pos_old = pos;

  for (;;) {
    size_t l = HEAP_CHILD_LEFT(pos);
    if (l >= len) {
      break;
    }
    size_t r = HEAP_CHILD_RIGHT(pos);
    size_t j = (r < len && __less(h[r].val, h[l].val)) ? r : l;
    if (!(__less(h[j].val, t.val))) {
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

template <typename T, typename Compare = std::less<T>>
void heap_fix(HeapEntry<T> *h, size_t len, size_t pos) {
  if (!heap_down<T, Compare>(h, pos, len)) {
    heap_up<T, Compare>(h, pos);
  }
}
