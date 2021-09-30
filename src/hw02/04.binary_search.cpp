#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

int binarySearch(std::vector<int> numbers, int target)
{
    int left = 0, right = numbers.size();
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (numbers[mid] == target)
            return mid;
        else if (numbers[mid] > target)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return -1;
}

TEST_CASE("二分查找")
{
    CHECK_EQ(binarySearch({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 5), 5);
    CHECK_EQ(binarySearch({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 6), 6);
    CHECK_EQ(binarySearch({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10), -1);
}