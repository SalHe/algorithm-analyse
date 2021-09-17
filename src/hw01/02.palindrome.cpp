#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

using namespace std;

int adjustPalinedrome(vector<int> &nums, int start, int end)
{
    int size = end - start + 1;
    if (size <= 1)
        return 0;

    if (nums[start] == nums[end])
    {
        return adjustPalinedrome(nums, start + 1, end - 1);
    }
    else if (nums[start] < nums[end])
    {
        int newNum = nums[start] + nums[start + 1];
        nums.erase(nums.begin() + start);
        nums[start] = newNum;
        end--;
        return 1 + adjustPalinedrome(nums,
                                     nums[start] == nums[end] ? start + 1 : start,
                                     nums[start] == nums[end] ? end - 1 : end);
    }
    else
    {
        int newNum = nums[end] + nums[end - 1];
        nums.pop_back();
        end--;
        nums[end] = newNum;
        return 1 + adjustPalinedrome(nums,
                                     nums[start] == nums[end] ? start + 1 : start,
                                     nums[start] == nums[end] ? end - 1 : end);
    }

    return 0;
}

vector<int> adjustPalinedrome(vector<int> nums)
{
    auto x = adjustPalinedrome(nums, 0, nums.size() - 1);
    return nums;
}

TEST_CASE("调整回文数")
{
    CHECK_EQ(adjustPalinedrome({1, 2, 3, 4, 6}), /* result: */ vector<int>({6, 4, 6}));
    CHECK_EQ(adjustPalinedrome({6, 4, 3, 2, 1}), /* result: */ vector<int>({6, 4, 6}));
    CHECK_EQ(adjustPalinedrome({3, 3, 4, 4, 6}), /* result: */ vector<int>({6, 4, 4, 6}));
    CHECK_EQ(adjustPalinedrome({1, 2, 3, 1, 2}), /* result: */ vector<int>({3, 3, 3}));
}