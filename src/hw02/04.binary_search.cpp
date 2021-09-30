#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <tuple>

using namespace std;
using Result = tuple<int, int>;

#define FOUND(p1, p2) ((Result){p1, p2})
#define FOUND_ONE(p) FOUND(p, p)
#define NOT_FOUND ((Result){})

Result binarySearch(vector<int> numbers)
{
    int left = 0, right = numbers.size();
    int mid = 0;
    while (left <= right)
    {
        mid = left + (right - left) / 2;
        if (numbers[mid] == mid)
            break;
        else if (numbers[mid] > mid)
            right = mid - 1;
        else
            left = mid + 1;
    }

    if (mid == numbers[mid])
    {
        right = numbers.size() - 1;
        if (numbers[right] != right)
        {
            right = mid;
            while (right == numbers[right])
                right++;
            right--;
        }

        left = 0;
        if (numbers[left] != left)
        {
            left = mid;
            while (left == numbers[left])
                left--;
            left++;
        }
        return FOUND(left, right);
    }
    return NOT_FOUND;
}

TEST_CASE("二分查找")
{
    SUBCASE("单个解")
    {
        CHECK_EQ(binarySearch({1, 2, 3, 3, 5, 6, 7}), FOUND_ONE(3));
    }

    SUBCASE("多个解")
    {
        CHECK_EQ(binarySearch({1, 2, 3, 3, 4, 5, 7}), FOUND(3, 5));
        CHECK_EQ(binarySearch({1, 2, 3, 3, 4, 5, 6}), FOUND(3, 6)); // 边界测试
        CHECK_EQ(binarySearch({0, 1, 2, 4, 5, 6, 7}), FOUND(0, 2)); // 边界测试
    }

    SUBCASE("无解")
    {
        CHECK_EQ(binarySearch({1, 2, 3, 4, 5, 6, 7}), NOT_FOUND);
    }
}