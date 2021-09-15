#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <algorithm>

using namespace std;

#define NOT_FOUND 0

int findSumCombination(vector<int> &nums, int sum, vector<int> &curr, int numsPos)
{
    int currSum = 0;
    for_each(curr.begin(), curr.end(),
             [&](int x)
             { currSum += x; });
    if (currSum == sum)
        return curr.size();
    if (numsPos >= nums.size())
        return NOT_FOUND;

    for (int i = numsPos; i < nums.size(); i++)
    {
        curr.push_back(nums[i]);
        int ret = findSumCombination(nums, sum, curr, numsPos + 1);
        curr.pop_back();
        if (ret)
            return ret;
    }
    return NOT_FOUND;
}

int findSumCombination(vector<int> nums, int sum)
{
    // 首先对给定的元素排序，
    // 具体排序算法暂不考
    sort(nums.begin(), nums.end());

    vector<int> curr;

    return findSumCombination(nums, sum, curr, 0);
}

TEST_CASE("求和")
{
    CHECK_EQ(findSumCombination({1, 2, 3, 4}, 6), /* result: */ 3);
    CHECK_EQ(findSumCombination({4, 2, 1, 3}, 6), /* result: */ 3);
    CHECK_EQ(findSumCombination({3, 3, 1, 2}, 6), /* result: */ 3);
    CHECK_EQ(findSumCombination({1, 1, 1, 3, 2}, 6), /* result: */ 4);
}