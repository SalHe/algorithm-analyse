#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> exchangeCoins(int total, vector<int> kinds)
{
    // 实际算法部分
    typedef struct
    {
        int count;
        int coin;
    } trace_info;

    vector<trace_info> traces(total + 1, trace_info{total + 1 /* 这里传一个较大的值，方便后边淘汰掉这个选择 */});
    traces[0].count = 0;
    for (int t = 1; t <= total; t++)
    {
        for (auto &&coin : kinds)
        {
            int rest = t - coin;
            if (rest == 0)
            {
                traces[t].count = 1;
                traces[t].coin = coin;
                break;
            }
            else if (rest > 0 && traces[rest].count + 1 < traces[t].count)
            {
                traces[t].coin = coin;
                traces[t].count = traces[rest].count + 1;
            }
        }
    }

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

typedef struct
{
    vector<int> coins;
    int total;
    vector<int> combination;
} TestCase;

TEST_CASE("金钱兑换问题")
{
    vector<TestCase> testCases = {
        {{1, 3, 5, 6}, 10, {5, 5}},
        {{1, 3, 5, 6}, 8, {3, 5}},
        {{1, 3, 5, 6}, 7, {1, 6}},
    };
    for (auto &&testCase : testCases)
    {
        vector<int> result = exchangeCoins(testCase.total, testCase.coins);
        sort(result.begin(), result.end());
        sort(testCase.combination.begin(), testCase.combination.end());
        CHECK_EQ(result, testCase.combination);
    }
}