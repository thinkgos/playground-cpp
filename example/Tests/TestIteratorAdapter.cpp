#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iterator>
#include <list>

// https://cplusplus.com/reference/iterator/distance/
TEST_CASE("std::distance", "iterator::adapter::distance") {
  std::list<int> l = {10, 20, 30, 40, 50};

  std::list<int>::iterator it_begin = l.begin();
  std::list<int>::iterator it_end = l.end();

  size_t dist = std::distance(it_begin, it_end);
  REQUIRE(dist == 5);
}

// https://cplusplus.com/reference/iterator/advance/
TEST_CASE("std::advance", "iterator::adapter::advance") {
  std::list<int> l = {10, 20, 30, 40, 50};

  std::list<int>::iterator it = l.begin();
  std::advance(it, 3);

  REQUIRE(*it == 40);
}

// https://cplusplus.com/reference/iterator/inserter/
TEST_CASE("std::front_inserter", "iterator::adapter::front_inserter") {
  std::deque<int> src = {10, 20, 30, 40, 50};
  std::deque<int> dst = {1, 2, 3, 4, 5};

  std::deque<int>::iterator it = dst.begin();
  std::copy(src.begin(), src.end(), std::front_inserter(dst));

  std::vector<int> want = {50, 40, 30, 20, 10, 1, 2, 3, 4, 5};
  for (std::deque<int>::iterator it = dst.begin(); it != dst.end(); ++it) {
    REQUIRE(*it == want[std::distance(dst.begin(), it)]);
  }
}

// https://cplusplus.com/reference/iterator/inserter/
TEST_CASE("std::inserter", "iterator::adapter::inserter") {
  std::list<int> src = {10, 20, 30, 40, 50};
  std::list<int> dst = {1, 2, 3, 4, 5};

  std::list<int>::iterator it = dst.begin();
  std::advance(it, 3);
  std::copy(src.begin(), src.end(), std::inserter(dst, it));

  std::vector<int> want = {1, 2, 3, 10, 20, 30, 40, 50, 4, 5};
  for (std::list<int>::iterator it = dst.begin(); it != dst.end(); ++it) {
    REQUIRE(*it == want[std::distance(dst.begin(), it)]);
  }
}