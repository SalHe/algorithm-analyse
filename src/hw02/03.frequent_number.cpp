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

TEST_CASE("寻找出现次数超过一半的数")
{
    CHECK_EQ(findFrequentNumber({3, 1, 1, 2, 1, 2, 1, 1}), FOUND(1));
    CHECK_EQ(findFrequentNumber({3, 1, 1, 2, 3, 2}), NOT_FOUND);
}