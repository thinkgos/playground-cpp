#include <iostream>
#include <stdint.h>
#include <vector>

int main(int argc, char const *argv[]) {
  std::vector<uint8_t> vec;

  vec.resize(10);

  std::cout << "size: " << vec.size() << std::endl;
  std::cout << "capacity: " << vec.capacity() << std::endl;

  return 0;
}
