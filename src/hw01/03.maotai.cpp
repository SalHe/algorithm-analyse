#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "stocks.h"

TEST_CASE("茅台股票")
{
    // 来自题目给的样例
    vector<double> prices = {1755, 1749, 1724, 1699, 1671, 1693, 1799, 1750, 1690, 1700, 1699, 1635, 1620, 1619, 1548, 1570, 1625, 1665, 1595, 1596, 1586, 1558, 1622, 1618, 1658, 1670, 1669, 1633, 1634, 1662};
    CHECK_EQ(predictStock(prices), std::make_tuple(/* 单价涨幅： */ 128, /* 起始日 */ 4, /* 结束日 */ 6));
}