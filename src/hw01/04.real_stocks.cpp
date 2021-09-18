#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "stocks.h"

// 头文件"04.real_stocks.h"由脚本"04.spider.js"自动从腾讯股票爬取并生成，
// 该脚本并会自动使用暴力算法给出参考答案(使用暴力算法保证准确性)。
#include "04.real_stocks.h"

TEST_CASE("测试股票真实数据")
{
    vector<double> prices = ALL_STOCKS;
    CHECK_EQ(predictStock(prices), std::make_tuple(STOCK_BENEFIT, STOCK_START, STOCK_END));
}