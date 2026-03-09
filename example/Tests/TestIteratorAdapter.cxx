#include <algorithm>
#include <deque>
#include <gtest/gtest.h>
#include <iterator>
#include <list>

// https://cplusplus.com/reference/iterator/distance/
TEST(TestDistance, IteratorAdapterDistance) {
  std::list<int> l = {10, 20, 30, 40, 50};

  std::list<int>::iterator it_begin = l.begin();
  std::list<int>::iterator it_end = l.end();

  size_t dist = std::distance(it_begin, it_end);
  ASSERT_EQ(dist, 5);
}

// https://cplusplus.com/reference/iterator/advance/
TEST(TestAdvance, IteratorAdapterAdvance) {
  std::list<int> l = {10, 20, 30, 40, 50};

  std::list<int>::iterator it = l.begin();
  std::advance(it, 3);

  ASSERT_EQ(*it, 40);
}

// https://cplusplus.com/reference/iterator/inserter/
TEST(TestFrontInserter, IteratorAdapterFrontInserter) {
  std::deque<int> src = {10, 20, 30, 40, 50};
  std::deque<int> dst = {1, 2, 3, 4, 5};

  std::deque<int>::iterator it = dst.begin();
  std::copy(src.begin(), src.end(), std::front_inserter(dst));

  std::vector<int> want = {50, 40, 30, 20, 10, 1, 2, 3, 4, 5};
  for (std::deque<int>::iterator it = dst.begin(); it != dst.end(); ++it) {
    ASSERT_EQ(*it, want[std::distance(dst.begin(), it)]);
  }
}

// https://cplusplus.com/reference/iterator/inserter/
TEST(TestInserter, IteratorAdapterInserter) {
  std::list<int> src = {10, 20, 30, 40, 50};
  std::list<int> dst = {1, 2, 3, 4, 5};

  std::list<int>::iterator it = dst.begin();
  std::advance(it, 3);
  std::copy(src.begin(), src.end(), std::inserter(dst, it));

  std::vector<int> want = {1, 2, 3, 10, 20, 30, 40, 50, 4, 5};
  for (std::list<int>::iterator it = dst.begin(); it != dst.end(); ++it) {
    ASSERT_EQ(*it, want[std::distance(dst.begin(), it)]);
  }
}