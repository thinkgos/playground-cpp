#include <iostream>

#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
    std::vector<int> nums;

    nums.push_back(3);
    std::push_heap(nums.begin(), nums.end());
    nums.push_back(1);
    std::push_heap(nums.begin(), nums.end());
    nums.push_back(5);
    std::push_heap(nums.begin(), nums.end());
    nums.push_back(2);
    std::push_heap(nums.begin(), nums.end());

    for (; !nums.empty();)
    {
        std::pop_heap(nums.begin(), nums.end());
        auto last = nums.back();
        nums.pop_back();
        std::cout << last << std::endl;
    }

    return 0;
}
