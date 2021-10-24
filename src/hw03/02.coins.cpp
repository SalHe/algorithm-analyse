#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief 求解给定的价值的钱兑换的硬币数量最少的方案。
 * 
 * @param total 目标价值。
 * @param kinds 硬币种类。
 * @return vector<int> 最优解中包含的所有硬币。
 */
vector<int> exchangeCoins(int total, vector<int> kinds)
{
    // 该结构用于记录价值$t$硬币组合的最少的个数以及最后一次拿取的硬币
    typedef struct
    {
        int count;
        int coin;
    } trace_info;

    // 此数组即为动态规划数组
    vector<trace_info> traces(total + 1, trace_info{total + 1 /* 这里传一个较大的值，方便后边淘汰掉这个选择 */});
    traces[0].count = 0; // 价值0不需要任何硬币进行组合

    // 由1到total求对应价值所需的最少硬币个数
    for (int t = 1; t <= total; t++)
    {
        // 求解最少消耗硬币个数
        for (auto &&coin : kinds)
        {
            int rest = t - coin;
            if (rest == 0)
            {
                // 如果当前硬币与total等值，那么只需要该硬币即可
                traces[t].count = 1;
                traces[t].coin = coin;
                break;
            }
            else if (rest > 0 && traces[rest].count + 1 < traces[t].count)
            {
                // 否则，如果拿下当前硬币后还需要补充硬币的话，那么我们总是保留硬币个数最少的情况
                traces[t].coin = coin;
                traces[t].count = traces[rest].count + 1;
            }
        }
    }

    // 回溯求解
    vector<int> coins;
    {
        int solution = total;
        while (solution > 0)
        {
            coins.push_back(traces[solution].coin);
            solution -= traces[solution].coin;
        }
    }
    return coins;
}

/**
 * @brief 测试用例
 * 
 */
typedef struct
{
    vector<int> coins;       // 硬币总类
    int total;               // 要求的目标价值
    vector<int> combination; // 预期的最佳解
} TestCase;

TEST_CASE("金钱兑换问题")
{
    // 本程序所有测试用例
    // 某些情况下不唯一，所以测试用例给的是存在唯一解方便自动化测试
    vector<TestCase> testCases = {
        {{1, 3, 5, 6}, 10, {5, 5}},
        {{1, 3, 5, 6}, 8, {3, 5}},
        {{1, 3, 5, 6}, 7, {1, 6}},
    };

    // 对每个测试用例通过算法求最佳解，并与我们给的参考解（预期的最佳解）做对比。
    for (auto &&testCase : testCases)
    {
        vector<int> result = exchangeCoins(testCase.total, testCase.coins);

        // 此处对结果、参考解排序是为了方便对比他们包含的元素是否一致
        sort(result.begin(), result.end());
        sort(testCase.combination.begin(), testCase.combination.end());

        // 对比算法求出来的结果与参考解
        CHECK_EQ(result, testCase.combination);
    }
}