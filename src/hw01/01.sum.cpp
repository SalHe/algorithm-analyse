#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>

using namespace std;
using Ints = vector<int>;
using Combinations = vector<Ints>;
using Result = tuple<int, Combinations>;

void findSumCombination(Ints &nums, int sum, Ints &curr, int numsPos, Combinations &combinations, int &max)
{
    int currSum = 0;
    for_each(curr.begin(), curr.end(),
             [&](int x)
             { currSum += x; });
    if (currSum > sum)
    {
        return;
    }
    else if (currSum == sum)
    {
        if (curr.size() > max)
        {
            max = curr.size();
            combinations.clear();
            combinations.push_back(curr);
        }
        else if (curr.size() == max)
        {
            combinations.push_back(curr);
        }
        return;
    }
    if (numsPos >= nums.size())
        return;

    for (int i = numsPos; i < nums.size(); i++)
    {
        // TODO 考虑如何去重
        // if (numsPos && nums[i] == curr[numsPos - 1])
        //     continue;
        curr.push_back(nums[i]);
        findSumCombination(nums, sum, curr, i + 1, combinations, max);
        curr.pop_back();
    }
}

Result findSumCombination(Ints nums, int sum)
{
    // 首先对给定的元素排序，
    // 具体排序算法暂不考虑
    sort(nums.begin(), nums.end());

    Ints curr;
    Combinations combinations;
    int max = 0;

    findSumCombination(nums, sum, curr, 0, combinations, max);
    return {max, combinations};
}

TEST_CASE("求和")
{
    CHECK_EQ(
        findSumCombination({1, 2, 3, 4}, 6),

        (Result){
            /* 序列中数字个数 */ 3,
            /* 可能的结果集，遵循字典序 */
            {
                {1, 2, 3},
            },
        });
    CHECK_EQ(
        findSumCombination({4, 2, 1, 3}, 6),

        (Result){
            /* 序列中数字个数 */ 3,
            /* 可能的结果集，遵循字典序 */
            {
                {1, 2, 3},
            },
        });
    CHECK_EQ(
        findSumCombination({3, 3, 1, 2}, 6),

        (Result){
            /* 序列中数字个数 */ 3,
            /* 可能的结果集，遵循字典序 */
            {
                {1, 2, 3},
                {1, 2, 3},
            },
        });
    CHECK_EQ(
        findSumCombination({1, 1, 1, 3, 2}, 6),

        (Result){
            /* 序列中数字个数 */ 4,
            /* 可能的结果集，遵循字典序 */
            {
                {1, 1, 1, 3},
            },
        });
    CHECK_EQ(
        findSumCombination({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2}, 15),

        (Result){
            /* 序列中数字个数 */ 5,
            /* 可能的结果集，遵循字典序 */
            {
                {1, 2, 2, 3, 7},
                {1, 2, 2, 4, 6},
                {1, 2, 3, 4, 5},
                {1, 2, 3, 4, 5},
            },
        });
}