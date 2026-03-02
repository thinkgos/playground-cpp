#include <catch2/catch_test_macros.hpp>

unsigned int Add(unsigned int v1, unsigned int v2) { return v1 + v2; }

TEST_CASE("Add are computed1", "[add1]") {
  REQUIRE(Add(1, 2) == 3);
  REQUIRE(Add(2, 3) == 5);
  REQUIRE(Add(3, 4) == 7);
  REQUIRE(Add(10, 20) == 30);
}

TEST_CASE("Add are computed2", "[add2]") {
  REQUIRE(Add(1, 2) == 3);
  REQUIRE(Add(2, 3) == 5);
  REQUIRE(Add(3, 4) == 7);
  REQUIRE(Add(10, 20) == 30);
}