#include "heap.h"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

class TestHeap : public ::testing::Test {
protected:
  std::vector<HeapEntry<uint64_t>> m_vec;
  std::vector<size_t> m_heap_index = {0, 0, 0, 0, 0, 0, 0, 0};

  TestHeap() {
    m_vec.push_back(HeapEntry(&m_heap_index[0], (uint64_t)200));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[1], (uint64_t)700));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[2], (uint64_t)600));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[3], (uint64_t)800));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[4], (uint64_t)100));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[5], (uint64_t)400));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[6], (uint64_t)300));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());

    m_vec.push_back(HeapEntry(&m_heap_index[7], (uint64_t)500));
    heap_push(m_vec.data(), m_vec.size(), std::greater<uint64_t>());
  }
  size_t size() const noexcept { return this->m_vec.size(); }
  bool is_heap() const noexcept {
    return std::is_heap(m_vec.begin(), m_vec.end(), [](auto &a, auto &b) {
      return std::greater<uint64_t>()(a.val, b.val);
    });
  }
  void debug_print() const noexcept {
    for (auto &a : this->m_vec) {
      std::cout << a.val << std::endl;
    }
  }
};

TEST_F(TestHeap, KeepHeap) { ASSERT_TRUE(this->is_heap()); }

TEST_F(TestHeap, AdjustToSiftUp) {
  size_t pos = this->m_heap_index[4];
  this->m_vec[pos] = HeapEntry(&this->m_heap_index[4], (uint64_t)20);
  heap_fix(this->m_vec.data(), this->m_vec.size(), pos,
           std::greater<uint64_t>());

  ASSERT_TRUE(this->is_heap());
  ASSERT_EQ(this->m_vec[0].val, (uint64_t)20);
  ASSERT_EQ(this->size(), (size_t)8);
}

TEST_F(TestHeap, AdjustToSiftDown) {
  size_t pos = this->m_heap_index[4];
  this->m_vec[pos] = HeapEntry(&this->m_heap_index[4], (uint64_t)1000);
  heap_fix(this->m_vec.data(), this->m_vec.size(), pos,
           std::greater<uint64_t>());

  ASSERT_TRUE(this->is_heap());
  ASSERT_EQ(this->size(), (size_t)8);
}

TEST_F(TestHeap, PopTop) {
  heap_pop(this->m_vec.data(), this->m_vec.size(), std::greater<uint64_t>());
  auto top = this->m_vec.back();
  this->m_vec.pop_back();

  ASSERT_TRUE(this->is_heap());
  ASSERT_EQ(top.val, (uint64_t)100);
  ASSERT_EQ(this->size(), (size_t)7);
}

TEST_F(TestHeap, PopPos) {
  size_t pos = this->m_heap_index[4];
  uint64_t want = this->m_vec[pos].val;
  heap_pop(this->m_vec.data(), this->m_vec.size(), pos,
           std::greater<uint64_t>());

  auto got = this->m_vec.back();
  this->m_vec.pop_back();

  ASSERT_TRUE(this->is_heap());
  ASSERT_EQ(want, got.val);
}