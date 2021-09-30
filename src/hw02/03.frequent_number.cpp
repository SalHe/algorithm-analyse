#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <limits>
#include <tuple>

using Number = int;
using Numbers = std::vector<Number>;

/**
 * @brief 找到的结果。第一个元素代表是否找到了，第二个元素代表找到的值（如果找到了）。
 * 
 */
using Result = std::tuple<bool, Number>;
const Result NOT_FOUND = {false, 0};
#define FOUND(x) ((Result){true, x})

/**
 * @brief 查找出现次数超过一半的数。摩尔投票算法。
 * 
 * @param numbers 
 * @return Result 
 */
Result findFrequentNumber(Numbers numbers)
{
    if (numbers.size() <= 0)
        return NOT_FOUND;

    int count = 0;
    int candicate = numbers[0];
    for (auto &&i : numbers)
    {
        if (candicate == i)
            count++;
        else if (count == 0)
            candicate = i;
        else
            count--;
    }

    count = 0;
    for (auto &&i : numbers)
    {
        if (i == candicate)
            count++;
    }

    if (count > numbers.size() / 2)
        return FOUND(candicate);

    return NOT_FOUND;
}

/**
 * @brief 查找出现次数超过一半的数。位运算法。
 *
 * 个人感觉这个算法设计得很精巧，但是不如摩尔投票法。
 * 
 * 参考：https://blog.csdn.net/qq_33880027/article/details/90764752
 * 
 * 该参考文档中的代码存在一些问题，已做出修正：
 * 1. 无法解决目标数为0的情况。
 * 2. 无法描述解的情况（是否有解）。
 * 3. 原给定的结果可能不是解
 * 
 * @param numbers 
 * @return Result 
 */
Result findFrequentNumber2(std::vector<int> numbers)
{
    std::vector<int> bits(32, 0);
    int res = 0;
    int countForZero = 0;
    for (int i : numbers)
    {
        if (i == 0)
        {
            countForZero++;
            continue;
        }
        int mask = 0x1;
        for (int j = 0; j < 32; j++)
        {
            bits[j] += (i & mask) >> j;
            mask <<= 1;
        }
    }
    bool found = false;
    if (countForZero > numbers.size() / 2)
    {
        return FOUND(0);
    }
    for (int i = 0; i < 32; i++)
    {
        if (bits[i] > numbers.size() / 2)
        {
            res += 1 << i;
            found = true;
        }
    }
    int count = 0;
    for (auto &&i : numbers)
    {
        if (i == res)
            count++;
    }

    if (!found || count < numbers.size() / 2)
        return NOT_FOUND;
    return FOUND(res);
}

TEST_CASE("寻找出现次数超过一半的数")
{
    typedef struct
    {
        std::vector<int> numbers;
        Result expect;
    } TestCase;
    std::vector<TestCase> testCases = {
        {{3, 1, 1, 2, 1, 2, 1, 1}, FOUND(1)},
        {{3, 1, 1, 2, 3, 2}, NOT_FOUND},
        {{0, 2, 0, 0, 1, 0, 1, 0, 1, 1, 0, 3, 0}, FOUND(0)},
    };

    for (auto &&testCase : testCases)
    {
        CHECK_EQ(findFrequentNumber(testCase.numbers), testCase.expect);
        CHECK_EQ(findFrequentNumber2(testCase.numbers), testCase.expect);
    }
}