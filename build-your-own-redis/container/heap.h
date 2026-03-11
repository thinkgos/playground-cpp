#pragma once

#include <algorithm>
#include <functional>
#include <stddef.h>
#include <utility>

template <typename T> struct HeapEntry {
  size_t *ref_pos = nullptr; // 记录下标，用于快速定位到堆中的位置
  T val = T{};               // 值

  HeapEntry() = default;
  HeapEntry(size_t *ref_pos, T val) : ref_pos(ref_pos), val(val) {}
};

template <typename T, typename Compare>
void __heap_sift_up(HeapEntry<T> h[], size_t pos, Compare __comp) {
  HeapEntry<T> hole = std::move(h[pos]); // record the hole value

  while (pos > 0) {
    size_t parent_pos = (pos - 1) / 2;
    if (!(__comp(h[parent_pos].val, hole.val))) {
      break;
    }
    // 上浮
    h[pos] = std::move(h[parent_pos]); // swap with the parent
    *(h[pos].ref_pos) = pos;           // record the position
    pos = parent_pos;                  // update hole position
  }
  h[pos] = std::move(hole); // fill the hole
  *(h[pos].ref_pos) = pos;  // record the position
}

template <typename T, typename Compare>
bool __heap_sift_down(HeapEntry<T> h[], size_t len, size_t pos,
                      Compare __comp) {

  HeapEntry<T> hole = std::move(h[pos]);
  size_t old_pos = pos;

  for (;;) {
    size_t l = pos * 2 + 1;
    if (l >= len) {
      break;
    }
    size_t r = pos * 2 + 2;
    size_t j = (r < len && __comp(h[l].val, h[r].val)) ? r : l;
    if (!__comp(hole.val, h[j].val)) {
      break;
    }

    h[pos] = std::move(h[j]); // swap with the kid
    *(h[pos].ref_pos) = pos;  // record the position
    pos = j;                  // update hole position
  }
  h[pos] = std::move(hole); // fill the hole
  *(h[pos].ref_pos) = pos;  // record the position
  return pos > old_pos;
}

// 调整指定位置的元素, 确保堆性质
template <typename T, typename Compare = std::less<>>
void heap_fix(HeapEntry<T> h[], size_t len, size_t pos, Compare __comp = {}) {
  if (!__heap_sift_down(h, len, pos, __comp)) {
    __heap_sift_up(h, pos, __comp);
  }
}

// 调整尾部元素, 确保堆性质
template <typename T, typename Compare = std::less<>>
void heap_push(HeapEntry<T> h[], size_t len, Compare __comp = {}) {
  __heap_sift_up(h, len - 1, __comp);
}

// 弹出指定位置的元素, 确保堆性质, 将弹出的元素移至尾部.
template <typename T, typename Compare = std::less<>>
void heap_pop(HeapEntry<T> h[], size_t len, size_t pos, Compare __comp = {}) {
  if (pos == 0) {
    heap_pop(h, len, __comp);
  } else if (pos < len - 1) {
    std::swap(h[pos], h[len - 1]);
    heap_fix(h, len - 1, pos, __comp);
  }
}

// 弹出顶部元素, 确保堆性质, 将弹出的元素移至尾部.
template <typename T, typename Compare = std::less<>>
void heap_pop(HeapEntry<T> h[], size_t len, Compare __comp = {}) {
  std::swap(h[0], h[len - 1]);
  __heap_sift_down(h, len - 1, 0, __comp);
}
