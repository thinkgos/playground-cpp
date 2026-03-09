#include "lru.h"
#include <gtest/gtest.h>
#include <vector>

template <typename K, typename V>
bool check_lru(lru<K, V> &cache, std::vector<V> expected) {
  return cache.size() == expected.size() &&
         std::equal(cache.begin(), cache.end(), expected.begin(),
                    [](const auto &cache_val, const auto &expected_val) {
                      return cache_val.second == expected_val;
                    });
}

class TestLru : public testing::Test {
protected:
  lru<int, int> cache = lru<int, int>(10);

  TestLru() {
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.put(4, 4);
    cache.put(5, 5);
  }
};

TEST_F(TestLru, Size) { ASSERT_EQ(cache.size(), 5); }

TEST_F(TestLru, Usage) {
  ASSERT_TRUE(check_lru(cache, {5, 4, 3, 2, 1}));
  ASSERT_EQ(cache.get(6), std::nullopt);
  ASSERT_EQ(cache.get(4), std::optional<int>(4));
  ASSERT_EQ(cache.get(2), std::optional<int>(2));
  ASSERT_TRUE(check_lru(cache, {2, 4, 5, 3, 1}));
}
