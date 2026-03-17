
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

class background_task {
public:
  void operator()(const std::string &name) const {
    std::cout << std::this_thread::get_id() << " " << name << "\n";
  }
};

void oops() {
  std::string name = "World";
  std::thread t(background_task{}, name);

  t.detach();
}

int main() {
  oops();
  sleep(1);
}
