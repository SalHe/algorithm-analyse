#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <vector>

using namespace std;

double quickPowerNormal(double number, int n)
{
    if (n == 0)
        return 1;
    else if (n < 0)
        return 1 / quickPowerNormal(number, -n);
    else if (n % 2 == 1)
        return number * quickPowerNormal(number, n - 1);
    else
    {
        double one = quickPowerNormal(number, n / 2);
        return one * one;
    }
}

double quickPower(double number, int n)
{
    int absN = n > 0 ? n : -n;

    // 处理特殊情况
    if (number == 0)
        return 0;
    else if (number == 1)
        return 1;
    else if (number == -1)
        return absN % 2 == 1 ? -1 : 1;

    // 一般情况
    // 其实这两个函数可以合并，
    // 但是上面的判断在一次求幂中只可能用到一次，
    // 所以单独提出来了。
    return quickPowerNormal(number, n);
}

TEST_CASE("分治求幂")
{
    SUBCASE("特殊情况")
    {
        CHECK_EQ(quickPower(0, 15), 0);
        CHECK_EQ(quickPower(1, 15), 1);
        CHECK_EQ(quickPower(-1, 16), 1);
        CHECK_EQ(quickPower(-1, 15), -1);
        CHECK_EQ(quickPower(-1, -15), -1);
    }

    SUBCASE("一般情况")
    {
        CHECK_EQ(quickPower(2, 0), 1);
        CHECK_EQ(quickPower(2, 6), 64);
        CHECK_EQ(quickPower(2, 7), 128);
        CHECK_EQ(quickPower(2, 8), 256);
        CHECK_EQ(quickPower(2, -8), 1.0 / 256.0);
    }
}